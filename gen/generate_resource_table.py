from os.path import exists
import json

# TODO:
# |-- Load JSON, XAML, YAML, TOML, INI, or some format to parse the resource table header definitions.
# `-- Use Resource Table Headers data to generate a C definition using templates with replaceable text.

# Also just thought of the fact that these resources will be embedded in the bootloader executable.
#     This has it's advantages and disadvantages; we'll see how exactly it pans out.
#     It may be necessary to be able to specify run-time files to find and load using UEFI,
#     but that is a whoooole can-o-worms I don't want to even think about right now.

def main():
    # Look for `table.json`
    if not exists("table.json"):
        print("Could not find \"table.json\", exiting.")
        return 1
        
    resource_table_file = open("table.json", "rt")
    resource_table_json = json.loads(resource_table_file.read())
    resource_table_file.close()

    if not resource_table_json["resource-table"]:
        print("Could not find \"resource-table\" entry in JSON.")
        return 1

    premade = [
        "memory-map",
        "gfx",
        "acpi",
    ]

    resource_table_json = resource_table_json["resource-table"]
    for key, value in resource_table_json.items():
        # TODO:
        # `-- Create list of resource table entries, with metadata.
        #     |-- Check for pre-defined entries.
        #     `-- Parse user-created entries.
        if key in premade:
            print("found recognized pre-made resource table entry:", key)
        else:
            # Parse custom resource table entry
            # Ensure valid signature
            if not value["signature"]:
                print("Table entry definition missing signature:", key)
                return 1

            if not len(value["signature"]) == 16:
                print("Table entry definition has invalid-sized signature:", key)
                print("  The signature should be 16 bytes long")
                return 1

            # Ensure valid contents
            if not value["contents"]:
                print("Table entry definition missing contents:", key)
                return 1

            try:
               byteCount = int(value["contents"])
               print("Found header with empty byte allocation:", key)
            except ValueError:
                # TODO: Somehow pass file contents on to be able to be used for definition.
                if not exists(value["contents"]):
                    print("Could not find file at", value["contents"],"| contents of", key)
                    return 1

                print("Found header with file path as contents:", key)

            pass


if __name__ == '__main__':
    main()
