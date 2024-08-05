# Tomeo Project Configuration Guide

This instruction guide will help you configure and run the Tomeo project in Qt Creator.

## Prerequisites

- **Qt Creator**: Ensure you have Qt version 5.15.2 installed.

## Steps to Configure the Project

1. **Open Qt Creator**:
   - Open the project by configuring the `.pro` file using Qt version: 5.15.2.

2. **Initial Run Error**:
   - When you click on run, you will get an error message saying "No videos Found!".

3. **Fixing the Video Path Error**:
   - Open one of the `.cpp` files (e.g., `the_button.cpp`), right-click on it, and copy the full path.
   - For example, if you copied the path `home/cunix/sc21sd/UI/source code/the_button.cpp`, navigate to `Projects -> Run -> Command line arguments (CLI)` and paste the path there.
   - Modify the CLI to: `home/cunix/sc21sd/UI/source code/videos`.

4. **Run the Project Again**:
   - Now, run the project again.

5. **Fixing Button Icon Issues**:
   - If some buttons do not have icons, open `the_player.h` file.
   - Find the `QIcon` line that contains the user directory and change that directory according to your preference.

## Additional Information

For more details on the development process and iterations of the Tomeo project, refer to the [project report](https://drive.google.com/file/d/1qF6bh8caatvQJdQ_bRcZNd8Ic45I968U/view?usp=sharing).

---

By following these steps, you should be able to configure and run the Tomeo project successfully.
