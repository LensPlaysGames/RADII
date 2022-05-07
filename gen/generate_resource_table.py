from os.path import exists, getsize
import json

# TODO:
# |-- Expand `table.json` to `config.json`, and add option for
# |   "kernel-virtual", "kernel-physical" addresses for ELF loader.
# |-- Honor efi-file runtime load requests.
# |   `-- add_efi_files() <- generated functions get called from in here.
# |       Entry format for each file: { ResourceTableHeader + Contents pointer }
# `-- Use Resource Table Headers data to generate a C definition.

class ResourceTableFileEntry:
    signature="                "
    contents=""

    def __init__(self, signature, contents = ""):
        self.signature = signature
        self.contents = contents

    def __str__(self):
        return "signature: \"" + self.signature + "\", contents: \"" + str(self.contents) + "\""

    def __repr__(self):
        return "<ResourceTableFileEntry " + self.__str__() + ">"


def main():
    if not exists("table.json"):
        print("Could not find \"table.json\", exiting.")
        return 1

    resource_table_file = open("table.json", "rt")
    resource_table_json = json.loads(resource_table_file.read())
    resource_table_file.close()

    if not resource_table_json["resource-table"]:
        print("Could not find \"resource-table\" entry in JSON.")
        return 1

    # Recognized keys
    # "memory-map" -- EFI memory map.
    # "gfx"        -- src/boot_information.h GraphicalInformation struct.
    # "acpi"       -- Pointer to RSDP 2.0 table header if present, NULL if not found.
    # "efi-file"   -- Generate code to load a file using EFI_SIMPLE_FILESYSTEM_PROTOCOL at runtime.

    resource_table_layout = []
    resource_table_json = resource_table_json["resource-table"]
    for key, value in resource_table_json.items():
        # TODO:
        # `-- Create list of resource table entries, with metadata.
        #     |-- Check for pre-defined entries.
        #     `-- Parse user-created entries.
        if key == "memory-map" \
           or key == "gfx"     \
           or key == "acpi":
            resource_table_layout.append(key)
        else:
            # Parse custom resource table entry
            # Ensure valid signature (existence + length)
            if not value["signature"]:
                print("Table entry definition missing signature:", key)
                return 1

            if not len(value["signature"]) == 16:
                print("Table entry definition has invalid-sized signature:", key)
                print("  The signature should be 16 bytes long")
                return 1

            # Ensure that contents exist
            if not value["contents"]:
                print("Table entry definition missing contents:", key)
                return 1

            resource_table_layout.append(
                ResourceTableFileEntry(
                    value["signature"],
                    value["contents"]
                )
            )

    # Open resource table header for writing.
    resource_table_header = open("resource_table.h", "wt")
    # Write start of header include gaurd.
    resource_table_header.write(
        "#ifndef RADII_RESOURCE_TABLE_H\n"
        + "#define RADII_RESOURCE_TABLE_H\n"
    )
    # Write #includes.
    resource_table_header.write(
        "#include <boot_information.h>\n"
        + "#include <EFI/types.h>\n"
    )
    # Write custom struct declarations.
    for entry in resource_table_layout:
        if entry == "memory-map" \
           or entry == "gfx"     \
           or entry == "acpi":
            print("premade:", entry)
            pass
        else:
            print("custom:", entry)
            byte_count = -1
            try:
                byte_count = int(entry.contents)
            except ValueError:
                if not exists(entry.contents):
                    print("Could not find file at", entry.contents, "| signature of", entry.signature)
                    return 1

                byte_count = getsize(entry.contents)

            if byte_count == -1:
                print("Invalid byte count for entry:", entry)
                return 1

            resource_table_header.write(
                "struct " + entry.signature + "_t {\n"
                "  ResourceTableHeader Header;\n"
                "  UINT8 Contents[" + str(byte_count) + "];\n"
                + "};\n"
            )

    # Write resource table declaration.
    resource_table_header.write("typedef struct resource_table_t {\n")
    for entry in resource_table_layout:
        if entry == "memory-map":
            resource_table_header.write("  MemoryInformation Memory;\n")
        elif entry == "gfx":
            resource_table_header.write("  GraphicalInformation Gfx;\n")
        elif entry == "acpi":
            resource_table_header.write("  VOID *RSDP;\n")
        else:
            resource_table_header.write("  struct " + entry.signature + "_t " + entry.signature + ";\n")

    resource_table_header.write("} ResourceTable;\n"
                                "extern ResourceTable resource_table;\n")

    # Finish up header include guard.
    resource_table_header.write("#endif /* #ifndef RADII_RESOURCE_TABLE_H */\n");
    # Close resource table header file; it's finished.
    resource_table_header.close()

    # Write resource table definition.
    resource_table_implementation = open("resource_table.c", "wt")
    resource_table_implementation.write("#include <resource_table.h>\n\n")
    resource_table_implementation.write("ResourceTable resource_table = {\n")
    for entry in resource_table_layout:
        if entry == "memory-map":
            resource_table_implementation.write(
                "  .Memory = {\n"
                "    .Header = {\n"
                "      .Signature = {'M','E','M','O','R','Y','M','A','P','S','M','E','M','O','R','Y'},\n"
                "      .Size = sizeof(ResourceTableHeader),\n"
                "      .Length = sizeof(MemoryInformation),\n"
                "      .Version = 0\n"
                "    }\n"
                "  },\n"
            )
        elif entry == "gfx":      pass
        elif entry == "acpi":     pass
        else:
            byte_count = -1
            # If is_file_path is True, entry.contents is treated as a filepath.
            # If is_file_path is False, entry.contents is treated as a byte count.
            is_file_path = False
            try:
                byte_count = int(entry.contents)
            except ValueError:
                byte_count = getsize(entry.contents)
                is_file_path = True

            resource_table_implementation.write("  ." + entry.signature + " = {\n")
            resource_table_implementation.write(
                "    .Header = {\n"
                "      .Signature = "
                "{'" + entry.signature[0] + "'"
                ",'" + entry.signature[1] + "'"
                ",'" + entry.signature[2] + "'"
                ",'" + entry.signature[3] + "'"
                ",'" + entry.signature[4] + "'"
                ",'" + entry.signature[5] + "'"
                ",'" + entry.signature[6] + "'"
                ",'" + entry.signature[7] + "'"
                ",'" + entry.signature[8] + "'"
                ",'" + entry.signature[9] + "'"
                ",'" + entry.signature[10] + "'"
                ",'" + entry.signature[11] + "'"
                ",'" + entry.signature[12] + "'"
                ",'" + entry.signature[13] + "'"
                ",'" + entry.signature[14] + "'"
                ",'" + entry.signature[15] + "'"
                "},\n"
                "      .Size = sizeof(ResourceTableHeader),\n"
                "      .Length = sizeof(ResourceTableHeader) + " + str(byte_count) + ",\n"
                "      .Version = 0\n"
                "    },\n"
            )
            # Write the correct amount of contents of file/only spaces.
            # And do it pretty!
            resource_table_implementation.write("    .Contents = {\n      ")
            newlineBucket = 6
            newlineThreshold = 72
            if not is_file_path:
                for i in range(byte_count):
                    resource_table_implementation.write("' ',")
                    newlineBucket += 4
                    if newlineBucket >= newlineThreshold:
                        resource_table_implementation.write("\n      ")
                        newlineBucket = 6
                resource_table_implementation.write("\n")
            else:
                readBytes = open(entry.contents, "rb").read()
                for i in range(getsize(entry.contents)):
                    to_write = str(readBytes[i]) + ","
                    newlineBucket += len(to_write)
                    if newlineBucket >= newlineThreshold:
                        resource_table_implementation.write("\n      ")
                        newlineBucket = 6
                    resource_table_implementation.write(to_write)

            resource_table_implementation.write("    },\n")
            resource_table_implementation.write("  },\n")

    resource_table_implementation.write("};\n")


if __name__ == '__main__':
    main()
