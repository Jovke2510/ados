import os 
import subprocess
import random

input_folder = "/home/lazar/Downloads/input_imgs"
output_folder = "/home/lazar/Downloads/aug_imgs"
prog_name = "Augmentation 4"

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

image_files = [f for f in os.listdir(input_folder) if f.endswith(('.png', '.jpg', '.jpeg'))]

for file_name in image_files:
    input_image = os.path.join(input_folder, file_name)
    output_image = os.path.join(output_folder, file_name)

    command = [
        './ImageDSP/build/linux/build-ImageDSP-Desktop_Qt_6_5_1_GCC_64bit-Debug/ImageDSP',
        '--prog-name', prog_name,
        '--in-img', input_image,
        '--out-img', output_image,
        '--params', 100, 0.1
    ]

    subprocess.run(command, check = False)

print("Augmentation finished")    