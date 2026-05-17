# System Monitor Tool

A lightweight system monitoring tool written in C for collecting and reporting basic Linux system information, including CPU usage, memory status, CPU core count, and core frequency data.

## Project Overview

This project is a low-level system monitoring utility designed to inspect system resource usage through Linux system interfaces. It reads system information from the `/proc` filesystem and related system files, then reports hardware and runtime metrics through a command-line interface.

The project is implemented in modular C, with separate components for CPU monitoring, memory monitoring, core count detection, and CPU frequency inspection.

## Features

- Monitor CPU utilization
- Retrieve memory usage information
- Detect CPU core count
- Read CPU core frequency information
- Modular C source structure
- Makefile-based build process
- Lightweight command-line execution

## Project Structure

```text
system-monitor-tool/
├── Config.h
├── Makefile
├── core_count.c
├── core_count.h
├── core_freq.c
├── core_freq.h
├── cpu.c
├── cpu.h
├── memory.c
├── memory.h
└── myMonitoringTool.c

## Run Instructions

After building, run the executable:

./myMonitoringTool

If permission is required, use:

chmod +x myMonitoringTool
./myMonitoringTool
