import os

fileDir = os.path.dirname(__file__)+'./avr_instruction.txt'

print(fileDir)

instructions = []
enums = []

# Open the file for reading
with open(fileDir, 'r') as file:

    cnt = 0
    # Iterate through each line in the file
    for line in file:

        # Split the line by '-'
        parts = line.strip().split('-')

        if len(line.replace(' ', '').replace('\r', '').replace('\n', '')) == 0:
            continue

        # Check if the first part contains binary data (0s and 1s)
        if len(parts) > 1 and (parts[0][0] == '0' or parts[0][0] == '1'):
            instruction = parts[0].strip().replace(' ', '')
            description = parts[1].strip().split(':')

            pattern = ''
            mask = ''

            for char in instruction:
                if char == '0' or char == '1':
                    mask += '1'
                    pattern += char
                else:
                    mask += '0'
                    pattern += '0'

            #Opcode ,mask, name, instructionSize, clock_count, description
            # Process or store instruction and description as needed
            element = 'm_instructionArray[instr_index++] = {0x0000, 0b' + pattern + ', 0b' + mask + \
                ', ' + 'InstructionAVR_en::' + description[0].strip() + ', 2, 1' + ', ' + \
                '"' + description[0].strip() + " : " + \
                description[1].strip() + '"' + '};'
            instructions.append(element)
            enums.append(description[0].strip())

            cnt += 1
        else:
            comment = "\r\n" + "//" + line.strip()
            instructions.append(comment)

    # Print the instructions and count
    for i, elem in enumerate(instructions, 1):
        print(f"{elem}")
    for i, elem in enumerate(enums, 1):
        print(f"{elem},")
    print(f"Total instructions: {cnt}")
