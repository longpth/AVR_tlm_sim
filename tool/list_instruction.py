import os

fileDir = '/media/sf_SystemC_shared/AVR_tlm_sim/samples/MEGA_LED_EXAMPLE1/MEGA_LED_EXAMPLE1/Debug/asm.log'

print(fileDir)

instructions = set()

# Open the file for reading
with open(fileDir, 'r') as file:

    for line in file:
        instruction = line.split(':')[1].replace(' ','')
        instructions.add(instruction)

print(sorted(instructions))
