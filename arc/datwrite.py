import sys
import os

def process_arc_file(input_file_path, dat_write_file, fileId):
    with open(input_file_path, "rb") as file:
        data = file.read()
    with open(dat_write_file, "rb") as file:
        data2 = file.read()

    ms = bytearray(data)
    ms2 = bytearray(data2)
    offset = 0
    sector = int.from_bytes(ms2[offset + fileId * 8:offset + 3 + fileId * 8], byteorder='little')
    size = int.from_bytes(ms2[offset + fileId * 8 + 4:offset + 3 + 4 + fileId * 8], byteorder='little')

    if len(ms) != size:
        print("ERROR: input file does match with DAT replace file")
        return
    ms2[sector * 0x800:sector * 0x800 + size] = ms

    with open(dat_write_file, "wb") as output_file:
        output_file.write(ms2)
    
    print("Program Completed, File - " + str(fileId) + " was written to the DAT file.")
    return


if len(sys.argv) != 4:
    print("Made by PogChampGuy AKA Kuumba")
    print("This Program is used for writing ARC files to the MegaMan X5/X6 DAT file")
    print("Usage: python datwrite.py <input_file_arc> <output_dat_file> <file_id>")
else:
    input_file_path = sys.argv[1]
    dat_write_file = sys.argv[2]
    idString = sys.argv[3]

    if not os.path.exists(input_file_path):
        print("ERROR: ARC file does not exist")
        exit(1)
    
    if not os.path.exists(dat_write_file):
        print("ERROR: DAT file does not exist")
        exit(1)
    
    id = -1

    if idString.startswith("0x"):
        id = int(idString.replace("0x",""),16)
    else:
        id = int(idString.replace("0x",""),10)

    process_arc_file(input_file_path, dat_write_file,id)
    