import os

fileDir = os.path.dirname(__file__)+'./avr_instruction.txt'

print(fileDir)

instructions = []
chars = []

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
                if (char != '0') and (char != '1') and (char not in chars):
                    chars.append(char)

            #Opcode ,mask, name, instructionSize, clock_count, description
            # Process or store instruction and description as needed
            element = '/*' + description[0].strip() + '*/'
            instructions.append(element)

            cnt += 1
        else:
            comment = "\r\n" + "//" + line.strip()
            instructions.append(comment)

    # Print the instructions and count
    for i, elem in enumerate(instructions, 1):
        print(f"{elem}")
    for i, c in enumerate(chars, 1):
        print(f"{c}")
    print(f"Total instructions: {cnt}")
