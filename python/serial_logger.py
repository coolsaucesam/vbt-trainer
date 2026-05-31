import serial
import serial.tools.list_ports
import csv 
import os
from datetime import datetime

BAUD_RATE = 115200 # baude rate means bits per second
OUTPUT_DIR = os.path.join(os.path.dirname(__file__), '..', 'data', 'sessions') # build sessions directory
#where data will be stored in vbt-trainer/data/sessions
HEADER_ROW = ["timestamp_ms", "raw_velocity", "smooth_velocity", "rep_num"]
# defines header row in csv. Must match serial output

def find_esp32_port():
    """
    Automatically finds the port which the ESP32 is connected to.
    """
    ports = serial.tools.list_ports.comports() #finds and lists all ports visible

    for port in ports:
        desc = port.description.lower() # gets the human-readable description of each port, in lower case
        if any(chip in desc for chip in ['cp210', 'ch340', 'uart', 'esp32']):
            print(f"Found ESP32 on {port.device} ({port.description})")

            return port.device
    
    print("Could not auto-detect ESP32") # in case detection fails

    # The followings a manual method from the user

    for i, port in enumerate(ports):
        print(f" [{i}] {port.device} - {port.description}")

    choice = int(input("Enter port number: "))
    
    return ports[choice].device

def create_session_files():
    """
    Creates the output directory if it doesn't exists, the builds timestamped
    file paths for this session's raw data and rep data
    """
    os.makedirs(OUTPUT_DIR, exist_ok=True) # creates the directory, and any missing parent directories

    timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M")
    # gets the current date and time formatted as Year-Month-Day_Hour-Minute

    # builds the raw and rep paths with timestamp formatted names

    raw_path = os.path.join(OUTPUT_DIR, f"{timestamp}_raw.csv")
    rep_path = os.path.join(OUTPUT_DIR, f"{timestamp}_reps.csv")

    return raw_path, rep_path

def parse_rep_summary(line):
    """

    """
