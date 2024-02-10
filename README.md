<div align="center">
<h1 align="center">
<br>TESTE DE FÁBRICA DO NERVOSINHO</h1>
<h3>Made for post-fab perfection</h3>
<h4>Developed with the software and tools below.</h4>

<p align="center">
<img src="https://img.shields.io/badge/C-A8B9CC.svg?style=flat-square&logo=C&logoColor=black" alt="C" />
<img src="https://avatars.githubusercontent.com/u/2528830?s=200&v=4" alt="OpenWRT" width="25" />
</p>
</div>

---

## 📖 Table of Contents 
- [📖 Table of Contents](#-table-of-contents) 
- [📍 Overview](#-overview)
- [📦 Features](#-features)
- [📂 repository Structure](#-repository-structure)
- [⚙️ Modules](#modules)
- [🚀 Getting Started](#-getting-started)
    - [🔧 Installation](#-installation)
    - [🤖 Running teste-de-fabrica-nervosinho](#-running-teste-de-fabrica-nervosinho)
- [🤝 Contributing](#-contributing)
---


## 📍 Overview

The teste-de-fabrica-nervosinho repository is designed to efficiently test a hardware system for its single-board performances. It integrates functionalities for conducting STM and modem tests, verifying Ethernet connections, checking connected devices, and assessing system response to various parameters. It also enables interaction with EEPROM, controlling write protection and batch numbers set. Tests are automated and error messages are logged for reference, providing a valuable tool for maintaining system health and diagnosing potential hardware issues.

---

## 📦 Features

|    | Feature            | Description                                                                                                        |
|----|--------------------|--------------------------------------------------------------------------------------------------------------------|
| ⚙️ | Architecture   | Code is structured in a modular format using C for a MIPS architecture. It includes components for testing, configurations, and communications. |
| 🔗 | Dependencies   | The system relies on the OpenWRT Linux GCC compiler. Dependencies also include system libraries and user-defined libraries.|
| 🧩 | Modularity     | The code is structured into separate .c and .h files, with functionalities grouped based on their application for easy integration and interchangeability.|
| 🔌 | Integrations   | The system appears to interface with hardware via serial communication, STM, and Ethernet. The presence of a Makefile suggests build process automation.|


---


## 📂 Repository Structure

```sh
└── teste-de-fabrica-nervosinho/
    ├── Makefile
    ├── build/
    │   └── JIGTest
    ├── inc/
    │   ├── communication.h
    │   ├── config.h
    │   ├── config_serial.h
    │   ├── misc.h
    │   └── testfunctions.h
    └── src/
        ├── communication.c
        ├── config_serial.c
        ├── main.c
        ├── misc.c
        └── testfunctions.c

```

---


## ⚙️ Modules

<details closed><summary>Root</summary>

| File                    | Summary                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| ---                     | ---                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        |
| [Makefile]({file_path}) | The code setup allows for compiling a C program, named JIGTest, for the MIPS architecture using the OpenWRT Linux GCC compiler. It gathers all.c files from the src directory. It includes custom directories and libraries from the OpenWRT staging directory and user-defined includes in inc. The build outputs are placed in the build directory with a log. Two additional actions, clean and viewlog, allow for cleaning the build directory and viewing the build log respectively. |

</details>

<details closed><summary>Src</summary>

| File                           | Summary                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           |
| ---                            | ---                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               |
| [main.c]({file_path})          | The code initializes a hardware test. In the initialization, it opens a log file, stops a process using stop_charlesgo(), activates a flash sequence in STM, sleeps for 30 secs, and establishes a communication setup. In the main function, after initialization, it proceeds to run a series of tests, logging the output in the initialized log file.                                                                                                                                                                                         |
| [misc.c]({file_path})          | This code provides a collection of functions for testing a system. Functions include recording the timestamp of the test, starting and stopping the charles_go service, initiating and halting a flash sequence on specific system pins (BOOT_STM32 & RESET_STM32), flashing STM, downloading a file from GitHub, conducting an ethernet ping test, and running a generic OS command. The result of each operation is returned for error checking.                                                                                                |
| [communication.c]({file_path}) | This code represents communication functions within a library for interacting with devices over serial ports. It includes functions for initializing communication, setting parameters, constructing, parsing, sending, and receiving messages. It also handles receiving, parsing, and sending AT commands to a modem. It includes error checking and logging, as well as handling timeouts in communication processes.                                                                                                                          |
| [config_serial.c]({file_path}) | The code provides two functions, open_serial and open_modem. Their principal tasks are to open a serial device or a modem through specified paths. They then set various attribute values like baud rate, data bits, parity bits, stop bits, and flow control. They also enable the receiver and set it to local mode, configure input/output, and apply these settings. If the device can't be opened or the settings can't be applied, appropriate error messages are shown.                                                                    |
| [testfunctions.c]({file_path}) | The provided code represents an automated testing routine for a device with STM, modem, Ethernet, and switch interfaces. It performs functions such as pinging devices, logging errors, testing Ethernet access (by pinging DNS server and downloading batch information), testing ports (by reading DHCP leases and pinging connected devices), and testing an STM interface. It also interacts with an EEPROM to disable/enable write protection and set batch numbers. Modem functionality is tested using AT commands and results are logged. |

</details>

<details closed><summary>Inc</summary>

| File                           | Summary                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  |
| ---                            | ---                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      |
| [testfunctions.h]({file_path}) | The code represents a collection of test functions for a particular system. In summary, it checks system functionalities such as EEPROM writing, testing Ethernet connections by pinging and downloading batch numbers, validating modem responses and SIM card presence, and identifying connected devices. Additionally, it involves testing a custom STM unit and a flash STM sequence, error logging with flexibility to avoid repetitive titles, and device pinging for connection integrity checks.                |
| [config.h]({file_path})        | The provided code defines constants for communication protocols within a software project. It specifies protocols such as buffer size, message types for errors, responses, timeouts, and various commands for different processes like resetting components, signal strength acquisition, firmware version and tests for different components. It also defines constants for callback function lengths, message lengths, and message status types for unused messages, waiting to send, and waiting for response.       |
| [communication.h]({file_path}) | The code is part of a communication module in a larger system that handles serial communication with an STM module and a modem. It defines a Message struct and provides functions for initializing communication, parsing, setting, and constructing messages, sending messages, and receiving responses from both the STM and the modem. These messages follow a pre-defined format identified by tokens. The communication module functions in tandem with a config_serial.h file.                                    |
| [config_serial.h]({file_path}) | The code in `config_serial.h` declares functions for handling serial communications via a modem or a general serial port. It includes the system libraries needed for these operations. Two main functionalities provided by this code are opening a serial port and opening a modem, both accepting a `serialport` parameter. It's part of a larger project structure managing serial communications, configurations, and test functions.                                                                               |
| [misc.h]({file_path})          | The code provided is from a header file misc.h of a software system. It declares various utility functions related to managing the system's services and operations. It includes functions for starting/stopping the charlesgo service, initiating/stopping a flash sequence, flashing STM through stm32flash library, downloading a batch file from a remote repository, running an ethernet ping operation, and executing OS commands. These functions likely support the main functionalities of the software system. |

</details>

---

## 🚀 Getting Started

***Dependencies***

Please ensure you have the following dependencies installed on your system:

`- ℹ️ OpenWRT build for mipsel gcc` [refer to CharlinhOS repo](https://gitlab.com/gabriel-technologia/iot/charlinhos)


### 🔧 Installation

1. Clone the teste-de-fabrica-nervosinho repository:
```sh
git clone ../teste-de-fabrica-nervosinho
```

2. Change to the project directory:
```sh
cd teste-de-fabrica-nervosinho
```

3. Change OPENWRT_PATH variable in the Makefile with the path to your openWRT build:
```sh
OPENWRT_PATH ?= ~/Documentos/charlinhos/openwrt/
```

### 🤖 Building and executing teste-de-fabrica-nervosinho

```sh
make all
```
The resulting binary is located in ./build/ named as JIGTest - the updated binary MUST be included in every new CharlinhOS version. To execute it in the Nervosinho device, simply run it:

```sh
/opt/gabriel/bin/JIGTest
```

The resulting test log is stored in the root directory as the root user and can be viewed through:

```sh
cat ~/logHardwareTest.txt
```

For further instructions for testing the hardware, [refer to the manual](https://www.notion.so/gabrieltecnologia/AutoTest-Manual-for-Nervosinhos-factory-production-552e86604c6e4ff3b97156bc8587b45e)

---

## 🤝 Contributing

How to contribute:

- **[Submit Pull Requests](https://github.com/local/teste-de-fabrica-nervosinho/blob/main/CONTRIBUTING.md)**: Review open PRs, and submit your own PRs.
- **[Join the Discussions](https://github.com/local/teste-de-fabrica-nervosinho/discussions)**: Share your insights, provide feedback, or ask questions.
- **[Report Issues](https://github.com/local/teste-de-fabrica-nervosinho/issues)**: Submit bugs found or log feature requests for LOCAL.

#### *Contributing Guidelines*

<details closed>
<summary>Click to expand</summary>

1. **Fork the Repository**: Start by forking the project repository to your GitHub account.
2. **Clone Locally**: Clone the forked repository to your local machine using a Git client.
   ```sh
   git clone <your-forked-repo-url>
   ```
3. **Create a New Branch**: Always work on a new branch, giving it a descriptive name.
   ```sh
   git checkout -b new-feature-x
   ```
4. **Make Your Changes**: Develop and test your changes locally.
5. **Commit Your Changes**: Commit with a clear and concise message describing your updates.
   ```sh
   git commit -m 'Implemented new feature x.'
   ```
6. **Push to GitHub**: Push the changes to your forked repository.
   ```sh
   git push origin new-feature-x
   ```
7. **Submit a Pull Request**: Create a PR against the original project repository. Clearly describe the changes and their motivations.

Once your PR is reviewed and approved, it will be merged into the main branch.

</details>

