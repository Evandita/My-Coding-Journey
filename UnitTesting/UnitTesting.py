import subprocess
import shlex
import time
import os

def run_c_program(user, c_code, timeout=1):
    # Save the C code to a file
    with open(user+".c", "w") as file:
        file.write(c_code)

    # Compile the C code
    compile_command = "gcc "+user+".c -o "+user
    compile_process = subprocess.run(shlex.split(compile_command), stderr=subprocess.PIPE)

    if compile_process.returncode != 0:
        print("Compilation error:")
        print(compile_process.stderr.decode())
        return

    # Run the compiled program with a timeout
    run_command = "./"+user if os.name != 'nt' else user+".exe"
    try:
        start_time = time.time()
        result = subprocess.run(shlex.split(run_command), stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout)
        end_time = time.time()
        elapsed_time = end_time - start_time

        # Check if the program timed out
        if elapsed_time >= timeout:
            print("Timeout reached ({} seconds)".format(timeout))
            return

        # Print the output of the C program
        print("Output:")
        print(result.stdout.decode())

        # Print any error messages from the C program
        if result.stderr:
            print("Error:")
            print(result.stderr.decode())

    except subprocess.TimeoutExpired:
        print("Timeout reached ({} seconds)".format(timeout))

    finally:
        # Clean up: remove temporary files
        os.remove(user+".c")
        if os.name != 'nt':
            os.remove(user)

# Example usage
c_code = """
#include <stdio.h>
int main() {
    int i = 1;
    while(i);
    return 0;
}
"""

run_c_program("evan", c_code, timeout=1)
