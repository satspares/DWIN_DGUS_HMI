# A short script to demonstrate CoolTerm scripting
#
# Author: Roger Meier, 06-11-2020
# CoolTerm version: 1.8.0

import CoolTerm
import sys
s = CoolTerm.CoolTermSocket()

# Get the ID of the first open window
ID = s.GetWindowID(0)
if ID < 0:
    print("No open windows")
    sys.exit()

# Open the serial port
if s.Connect(ID):
    
    # Send some data
    s.Write(ID, "Hello CoolTerm")
    s.WriteLine(ID, ", how are you?")
    s.WriteLine(ID, "At the next prompt, send me some data.")
    answer = input("Send some data to CoolTerm. Press ENTER to continue.")

    # Poll the port and move any data from the serial receive
    # buffer to the CoolTerm receive buffer
    s.Poll(ID)

    # See how much data is available
    i = s.BytesAvailable(ID)

    # Get a copy of the data in the receive buffer
    d1 = s.LookAhead(ID)
    print("I have received",i,"characters:",d1)

    # Read 5 bytes from the buffer and display as hex string
    d2 = s.ReadHex(ID, 5)
    i = s.BytesAvailable(ID)
    print("I grabbed the following 5 bytes from the buffer:",d2)
    print("There are",i,"characters left in the buffer.")

    # Read what's left in the buffer
    d1 = s.ReadAll(ID)
    print("I grabbed the following remaining characters from the buffer:",d1)

    # Close the port
    s.Disconnect(ID)

    # Disconnect from CoolTerm
    s.Close()

else:
    print("Not Connected")
