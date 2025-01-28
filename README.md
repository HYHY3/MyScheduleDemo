# MyScheduleDemo
a demo to manage schedule info

## License
This software is licensed under a **Non-Commercial Use Only License**. Commercial use is strictly prohibited. See the LICENSE file for more details.

## Manual
1. git clone this repo to your local PC.(eg, D:\temp)

   ![image](https://github.com/user-attachments/assets/7013e505-3c2e-446d-8bc2-767d0158732d)
2. create an ConsoleApplication project or other project whatever you like in visual studio 2022.
3. set Include Directories to find Schedule.h
   ![image](https://github.com/user-attachments/assets/b7dd5b76-eacb-4f4f-951d-7367bbbdee82)
4. set Additional Library Directories to find static library named Schedule.lib. (the same step for x64)
  ![image](https://github.com/user-attachments/assets/cf9868de-e3c0-485b-a38f-e7b9f9956895)
5. set Additional Dependencies to find static library named Schedule.lib. (the same step for x64)
  ![image](https://github.com/user-attachments/assets/927637fb-173f-405d-b064-fc4ab2851ed4)
6. set Platform Toolset to make Linking OK.(the static library named Schedule.lib is created with visual studio 2015(v140))
  ![image](https://github.com/user-attachments/assets/7e186fb1-caf4-45ed-95e6-546b5036b036)
7. set Target Platform version to build OK.(only support UTF-8 encoding characters for input and output)
   ![image](https://github.com/user-attachments/assets/a91e26fd-96e6-421b-b329-11cf7665bcb1)
8. the result
   ![image](https://github.com/user-attachments/assets/24379301-8b56-42a0-9ced-bf3069533a0b)


