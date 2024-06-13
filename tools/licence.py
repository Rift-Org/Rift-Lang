import os

# Define the string to be appended
header = """/////////////////////////////////////////////////////////////
///                                                       ///
///     ██████╗ ██╗███████╗████████╗                      ///
///     ██╔══██╗██║██╔════╝╚══██╔══╝                      ///
///     ██████╔╝██║█████╗     ██║                         ///
///     ██╔══██╗██║██╔══╝     ██║                         ///
///     ██║  ██║██║██║        ██║                         ///
///     ╚═╝  ╚═╝╚═╝╚═╝        ╚═╝                         ///
///     * RIFT CORE - The official compiler for Rift.     ///
///     * Copyright (c) 2024, Rift-Org                    ///
///     * License terms may be found in the LICENSE file. ///
///                                                       ///
/////////////////////////////////////////////////////////////

"""


directory = "../lib/"


for root, dirs, files in os.walk(directory):
    for file in files:
        file_path = os.path.join(root, file)
        
        try:
            
            with open(file_path, "r", encoding="utf-8", errors="replace") as f:
                content = f.read()
            
            
            modified_content = header + content
            
            
            with open(file_path, "w", encoding="utf-8") as f:
                f.write(modified_content)
        except UnicodeDecodeError as e:
            print(f"Skipping file '{file_path}' due to decoding error: {str(e)}")