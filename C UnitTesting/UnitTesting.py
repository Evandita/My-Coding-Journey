import os
import subprocess
import json

def run_c_programs(folder_path, test_cases_file, output_file):
    # Get a list of all files in the folder
    files = [f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))]

    # Filter out only C files
    c_files = [f for f in files if f.endswith(".c")]

    with open(test_cases_file, 'r') as f:
        test_cases = json.load(f)

    with open(output_file, 'w') as result_file:
        for c_file in c_files:
            c_program_path = os.path.join(folder_path, c_file)
            print(f"Running C program: {c_program_path}")

            # Compile the C program
            compile_result = subprocess.run(["gcc", "-o", "temp_executable", c_program_path], stderr=subprocess.PIPE, text=True)
            if compile_result.returncode != 0:
                print(f"  Compilation failed for {c_file}: {compile_result.stderr.strip()}")
                result_file.write(f"{c_file}: Compilation failed\n")
                continue

            # Initialize the score for the current C file
            score = 0

            for test_case in test_cases:
                input_data = test_case["input"]
                expected_output = test_case["output"]

                # Run the C program and capture the output
                process = subprocess.Popen(["./temp_executable"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
                actual_output, _ = process.communicate(input=input_data)
                actual_output = actual_output.strip()

                # Compare the output with the expected output
                if actual_output == expected_output:
                    print(f"  Test Passed: Input - {input_data}, Output - {actual_output}")
                    score += 1
                else:
                    print(f"  Test Failed: Input - {input_data}, Expected Output - {expected_output}, Actual Output - {actual_output}")

            # Write the score to the result file
            result_file.write(f"{c_file}: {score} out of {len(test_cases)} tests passed = {score/len(test_cases) * 100}%\n")

            print("")

    # Remove temporary executable
    os.remove("temp_executable")

# Replace 'path_to_folder' with the actual path to your folder containing C programs
folder_path = 'C Testing'

# Define the output file for storing scores
output_file = 'output_scores.txt'

# Define the test cases file
test_cases_file = 'test_cases.json'

run_c_programs(folder_path, test_cases_file, output_file)
