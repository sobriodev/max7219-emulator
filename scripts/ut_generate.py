from os import listdir
import re
import sys
import argparse 
  
UNIT_TEST_DIR = '../unit_test'
UNIT_TEST_HEADERS_FILE = 'ut.h'
UNIT_TEST_RUNNER_FILE = 'ut_runner.c'

# Regex
UNIT_TEST_SOURCE_REGEX = '^ut_.+\.c$'
UNIT_TEST_FUNCTION_REGEX = '^void UT_.+\(void\)$'
UNIT_TEST_HEADERS_START_LINE = '^.+Put tests declaration here.+$'
UNIT_TEST_HEADERS_END_LINE = '^.+End of the tests declaration.+$'
UNIT_TEST_RUNNER_START_LINE = '^.+UNITY_BEGIN().+$'
UNIT_TEST_RUNNER_END_LINE = '^.+UNITY_END().+$'

# Initialize CLI parser 
parser = argparse.ArgumentParser()
parser.add_argument('-d', '--debug', help = 'Enable debug messages', action='store_true') 
args = parser.parse_args()

# Turn on debug if needed 
if args.debug:
    DEBUG_MODE = True
else:
    DEBUG_MODE = False

# Function to generate os path
def MakePath(dir, file):
    return dir + '/' + file

# Make correct header and RUN_TEST lines
def MakeHeaderAndRunnerEntries(line):
    entries = {}
    entries['header'] = '{};'.format(line)

    # Get function name from line and pass it to the runner entry
    onlyName = line.split(' ')[1].split('(')[0]
    entries['runner'] = 'RUN_TEST({});'.format(onlyName)
    return entries
 
# Collect unit tests from source files
def CollectUnitTests():
    # Get valid source files from unit test directory
    sourceFiles = [file for file in listdir(UNIT_TEST_DIR) if re.search(UNIT_TEST_SOURCE_REGEX, file) and file != UNIT_TEST_RUNNER_FILE]

    if DEBUG_MODE:
        print('Found source files:')
        for i in sourceFiles:
            print(i)

    # Iterate over all files and find unit test definitions
    unitTests = {}
    for sourceFile in sourceFiles:
        fileHandle = open(MakePath(UNIT_TEST_DIR, sourceFile), 'r')

        # File descriptor in the dictionary
        fileDescriptor = sourceFile.split('.')[0].upper()

        # Iterate over all lines
        lines = fileHandle.readlines()
        for line in lines:
            line = line.strip()
            if re.search(UNIT_TEST_FUNCTION_REGEX, line):

                if DEBUG_MODE:
                    print('Found unit test: {}'.format(line))
                
                # If dictionary already contains descriptor append new line, otherwise create list with line
                if not fileDescriptor in unitTests:
                    unitTests[fileDescriptor] = [MakeHeaderAndRunnerEntries(line)]
                else:
                    unitTests[fileDescriptor].append(MakeHeaderAndRunnerEntries(line))
        
        fileHandle.close()
    return unitTests

# Update headers file
def UpdateHeadersFile(unitTests):
    headersHandle = open(MakePath(UNIT_TEST_DIR, UNIT_TEST_HEADERS_FILE), 'r')
    lines = headersHandle.readlines()
    headersHandle.close()

    # Find positions of the start/end comments
    for i in range(len(lines)):
        if re.search(UNIT_TEST_HEADERS_START_LINE, lines[i]):
            startIndex = i
        elif re.search(UNIT_TEST_HEADERS_END_LINE, lines[i]):
            endIndex = i

    if DEBUG_MODE:
        print('Headers file start index: {}, end index: {}'.format(startIndex, endIndex))

    # Clear everything between the lines
    del lines[startIndex+1:endIndex]

    # Build new headers list
    newLines = []
    for module in unitTests:
        newLines.append('\n/* {} */\n'.format(module)) # Module name comment
        for elem in unitTests[module]:
            newLines.append('{}\n'.format(elem['header'])) # Header
    newLines.append('\n') # Append missing trailing new line

    # Combine two lists
    lines[startIndex+1:startIndex+1] = newLines

    # Re-open the file in write mode and update it
    headersHandle = open(MakePath(UNIT_TEST_DIR, UNIT_TEST_HEADERS_FILE), 'w')
    for line in lines:
        headersHandle.write(line)
    headersHandle.close()

# Update runner file
def UpdateRunnerFile(unitTests):
    runnerHandle = open(MakePath(UNIT_TEST_DIR, UNIT_TEST_RUNNER_FILE), 'r')
    lines = runnerHandle.readlines()
    runnerHandle.close()

    # Find positions of the start/end comments
    for i in range(len(lines)):
        if re.search(UNIT_TEST_RUNNER_START_LINE, lines[i]):
            startIndex = i
        elif re.search(UNIT_TEST_RUNNER_END_LINE, lines[i]):
            endIndex = i

    if DEBUG_MODE:
        print('Runner file start index: {}, end index: {}'.format(startIndex, endIndex))

    # Clear everything between the lines
    del lines[startIndex+1:endIndex]

    # Build new headers list
    newLines = []
    for module in unitTests:
        newLines.append('\n\t/* {} */\n'.format(module)) # Module name comment
        for elem in unitTests[module]:
            newLines.append('\t{}\n'.format(elem['runner'])) # RUN_TEST
    newLines.append('\n') # Append missing trailing new line

    # Combine two lists
    lines[startIndex+1:startIndex+1] = newLines

    # Re-open the file in write mode and update it
    runnerHandle = open(MakePath(UNIT_TEST_DIR, UNIT_TEST_RUNNER_FILE), 'w')
    for line in lines:
        runnerHandle.write(line)
    runnerHandle.close()
        
if __name__ == '__main__':
    if DEBUG_MODE:
        print('Collecting unit tests...')
    unitTests = CollectUnitTests()

    if DEBUG_MODE:
        print('Updating headers file...')
    UpdateHeadersFile(unitTests)
    
    if DEBUG_MODE:
        print('Updating runner file...')
    UpdateRunnerFile(unitTests)

    if DEBUG_MODE:
        print('Done!')
