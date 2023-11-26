import os
import subprocess
import sys
import xml.etree.ElementTree as ET

intepreter = "tests/ic23int"

def run_tests(program):
    testsuite = ET.Element("testsuite", name="Test Suite", tests="0", failures="0", errors="0", skipped="0")
    for path, subdirs, files in os.walk("."):
        for name in files:
            if name.endswith(".in"):
                test_name = name[:-3]
                classname = path.replace('/', '.')[2:]
                testcase = ET.Element("testcase", name=test_name, classname=classname, time="0")
                expected_rc = int(open(os.path.join(path, test_name + ".rc"), "r").read())
                try:
                    ## check if is in folder lexer, or if lexer is in path
                    if "lexer" in path or "lexer" in program:
                        
                        actual_rc = subprocess.call([program, "-t"], stdin=open(os.path.join(path, name)), stdout=open(os.path.join(path, test_name + ".temp"), "w"), timeout=1)

                        if actual_rc != expected_rc:
                            failure = ET.Element("failure", message="Return code does not match expected value", type="AssertionError", text=f"Expected return code {expected_rc}, but got {actual_rc}")
                            testcase.append(failure)
                            testsuite.set("failures", str(int(testsuite.get("failures")) + 1))
                        elif expected_rc == 0:
                            diff_rc = subprocess.call(["diff", os.path.join(path, test_name + ".temp"), os.path.join(path, test_name + ".out")])
                            if diff_rc != 0:
                                failure = ET.Element("failure", message="Output does not match expected value", type="AssertionError", text="Output does not match expected value")
                                testcase.append(failure)
                                testsuite.set("failures", str(int(testsuite.get("failures")) + 1))
                    else:


                        actual_rc = subprocess.call([program], stdin=open(os.path.join(path, name)), stdout=open(os.path.join(path, test_name + ".temp"), "w"), timeout=1)


                        if actual_rc != expected_rc:
                            failure = ET.Element("failure", message="Return code does not match expected value", type="AssertionError", text=f"Expected return code {expected_rc}, but got {actual_rc}")
                            testcase.append(failure)
                            testsuite.set("failures", str(int(testsuite.get("failures")) + 1))
                        elif expected_rc == 0:
                            #check if os.path.join(path, name + ".stdin") exists
                            if os.path.isfile(os.path.join(path, test_name + ".stdin")):
                                int_rc = subprocess.call([intepreter, os.path.join(path, test_name + ".temp")],stdin=open(os.path.join(path, test_name + ".stdin")), stdout=open(os.path.join(path, test_name + ".temp2"), "w"), timeout=1)

                            else:
                                int_rc = subprocess.call([intepreter, os.path.join(path, test_name + ".temp")], stdout=open(os.path.join(path, test_name + ".temp2"), "w"), timeout=1)            # stdin=open(os.path.join(path, test_name + ".temp")),
                            
                            if int_rc != 0:
                                failure = ET.Element("failure", message="Return code does not match expected value", type="AssertionError", text=f"Expected return code {expected_rc}, but got {int_rc}")
                                testcase.append(failure)
                                testsuite.set("failures", str(int(testsuite.get("failures")) + 1))
                            
                            diff_rc = subprocess.call(["diff", os.path.join(path, test_name + ".temp2"), os.path.join(path, test_name + ".out")])
                            if diff_rc != 0:
                                failure = ET.Element("failure", message="Output does not match expected value", type="AssertionError", text="Output does not match expected value")
                                testcase.append(failure)
                                testsuite.set("failures", str(int(testsuite.get("failures")) + 1))
                except subprocess.TimeoutExpired:
                    failure = ET.Element("failure", message="Test timed out", type="TimeoutError", text="Test timed out")
                    testcase.append(failure)
                    testsuite.set("failures", str(int(testsuite.get("failures")) + 1))
                if len(testcase.findall("failure")) == 0:
                    testcase.set("time", "0.001")
                testsuite.set("tests", str(int(testsuite.get("tests")) + 1))
                testsuite.append(testcase)
                try:
                    os.remove(os.path.join(path, test_name + ".temp"))
                except:
                    pass
                try:
                    os.remove(os.path.join(path, test_name + ".temp2"))
                except:
                    pass

    with open("test-results.xml", "w") as f:
        f.write(ET.tostring(testsuite, encoding="unicode", method="xml"))
    print(f"Tests passed: {int(testsuite.get('tests')) - int(testsuite.get('failures'))}/{testsuite.get('tests')}")
    print(f"Failures: {testsuite.get('failures')}")
    # print names of failed tests from ET
    for test in testsuite.findall("testcase"):
        if len(test.findall("failure")) != 0:
            print(test.get("classname") + "." + test.get("name"))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: python3 test_runner.py <program to run>")
        exit(1)
    program = sys.argv[1]
    run_tests(program)