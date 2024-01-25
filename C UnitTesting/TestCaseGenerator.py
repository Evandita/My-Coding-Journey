import json
import subprocess
import random

def generate_random_input():
    # Generate random input values
    a = random.randint(-100, 100)
    b = random.randint(-100, 100)
    return f"{a} {b}"

def generate_test_cases(correct_c_program, num_inputs, output_file):
    test_cases = []

    for _ in range(num_inputs):
        input_data = generate_random_input()

        process = subprocess.Popen(["gcc", "-o", "temp_executable", correct_c_program], stdout=subprocess.PIPE, text=True)
        process.communicate()

        process = subprocess.Popen(["./temp_executable"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
        actual_output, _ = process.communicate(input=input_data)
        actual_output = actual_output.strip()

        test_cases.append({"input": input_data, "output": actual_output})

    with open(output_file, 'w') as f:
        json.dump(test_cases, f)

# Replace 'correct_program.c' with the actual path to your correct C program
correct_c_program = 'Answer.c'

# Define the number of random inputs to generate
num_inputs = 100  # You can adjust this value

# Define the output file for storing test cases
output_file = 'test_cases.json'

# Generate and write test cases to a file
generate_test_cases(correct_c_program, num_inputs, output_file)
