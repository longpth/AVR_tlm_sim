import os

fileDir = os.path.dirname(__file__)+'../build/log_copy.txt'

print(fileDir)

instructions_avr_sim = []
instructions_ref = []

# Open the file for reading
with open(fileDir, 'r') as file:

    cnt = 0
    # Iterate through each line in the file
    for line in file:
        # Split the line by '-'
        parts = line.strip().split(' ')
        instruction = parts[2].lower()
        instructions_avr_sim.append(instruction)

fileDir = os.path.dirname(__file__)+'../build/log_asm.txt'


# Use a set to store unique elements
unique_elements = set(instructions_avr_sim)

# Convert the set back to a list if needed
unique_list = list(unique_elements)

# Print the unique elements
print(len(unique_list))
print(sorted(unique_list))

print(fileDir)

# Open the file for reading
with open(fileDir, 'r') as file:

    cnt = 0
    # Iterate through each line in the file
    for line in file:
        # Split the line by '-'
        parts = line.strip().split(' ')
        instruction = parts[0].lower()
        instructions_ref.append(instruction)

for i in range(0, len(instructions_ref)):
    if instructions_avr_sim[i][0:2] != instructions_ref[i][0:2]:
        print('{} != {}'.format(instructions_avr_sim[i], instructions_ref[i]))
