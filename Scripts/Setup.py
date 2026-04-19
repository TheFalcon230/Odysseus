import os
import subprocess

import platform

from CheckPython import PythonConfiguration as PythonRequirements

# Make sure everything we need is installed
PythonRequirements.Validate()

from Vulkan import VulkanConfiguration as VulkanRequirements

# Change from Scripts directory to root
os.chdir('../')

VulkanRequirements.Validate()

if platform.system() == "Windows":
    print("\nRunning premake...")
    subprocess.call([os.path.abspath("Scripts/Win-GenProjects.bat"), "nopause"])