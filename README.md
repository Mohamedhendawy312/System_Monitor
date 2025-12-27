# Linux System Monitor

A terminal-based system monitor built with C++ and ncurses, similar to htop. Reads from the Linux `/proc` filesystem to display real-time system information.

<img src="images/monitor.png"/>

## Features

- **CPU Utilization** - Real-time CPU usage percentage
- **Memory Usage** - System memory utilization
- **Process List** - Running processes sorted by RAM usage
- **Process Details** - PID, user, CPU%, RAM, uptime, command
- **System Info** - OS name, kernel version, uptime

## How It Works

The monitor reads directly from Linux's `/proc` virtual filesystem:
- `/proc/stat` - CPU statistics
- `/proc/meminfo` - Memory information
- `/proc/uptime` - System uptime
- `/proc/[pid]/` - Per-process information

## Project Structure

```
├── src/
│   ├── main.cpp              # Entry point
│   ├── system.cpp            # System info aggregation
│   ├── processor.cpp         # CPU utilization
│   ├── process.cpp           # Individual process data
│   ├── linux_parser.cpp      # /proc filesystem parsing
│   ├── ncurses_display.cpp   # Terminal UI rendering
│   └── format.cpp            # Time formatting utilities
├── include/                  # Header files
├── images/
│   └── monitor.png           # Screenshot
└── CMakeLists.txt
```

## Dependencies

- Linux OS (reads from `/proc`)
- cmake >= 2.6
- make >= 4.1
- gcc/g++ >= 5.4
- ncurses

### Install ncurses (Linux)
```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## Building

```bash
git clone https://github.com/Mohamedhendawy312/System_Monitor.git
cd System_Monitor
mkdir build && cd build
cmake .. && make
./monitor
```

## Author

Mohamed Hendawy
