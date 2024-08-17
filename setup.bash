#!/usr/bin/env bash

install_linux_dependencies() {
    echo "Linux detected - Installing dependencies"
    sudo apt update
    sudo apt install -y cmake make pkg-config libglfw3-dev libglm-dev libglew-dev libgl-dev libassimp-dev

    # Python
    sudo apt install -y python3 python3-pip python3-venv
    python3 -m venv env
    source env/bin/activate
    pip3 install numpy pandas scipy
}

install_mac_dependencies() {
    echo "Mac detected - Installing dependencies"
    if ! [ -x "$(command -v brew)" ]; then
        echo "Error: brew is not installed. Would you like to install it? (y/n)"
        read -r response
        if [[ "$response" =~ ^([yY])$ ]]; then
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
            # Add Homebrew to the PATH if necessary
            echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
            eval "$(/opt/homebrew/bin/brew shellenv)"
        else
            echo "Exiting"
            exit 1
        fi
    fi
    brew install glfw glm glew assimp

    # Python
    brew install python3
    python3 -m venv env
    source env/bin/activate
    pip3 install numpy pandas scipy
}

install_windows_dependencies() {
    echo "Windows detected - Installing dependencies"
    # Check if running as admin
    if [ "$EUID" -ne 0 ]; then
        echo "Please run as administrator"
        exit 1
    fi
    # Check if choco is installed
    if ! [ -x "$(command -v choco)" ]; then
        echo "Chocolatey is not installed. Would you like to install it? (y/n)"
        read -r response
        if [[ "$response" =~ ^([yY])$ ]]; then
            # Install Chocolatey
            powershell -NoProfile -ExecutionPolicy Bypass -Command "Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))"
        else
            echo "Exiting"
            exit 1
        fi
    fi

    # Install dependencies using choco
    choco install cmake -y
    choco install make -y
    choco install glfw -y
    choco install glm -y
    choco install glew -y
    choco install assimp -y

    # Python
    choco install python3 -y
    python3 -m venv env
    source env/Scripts/activate
    pip3 install numpy pandas scipy
}

# Determine OS and install accordingly
OS_TYPE=$(uname -s)
case "$OS_TYPE" in
    Linux*)
        install_linux_dependencies
        ;;
    Darwin*)
        install_mac_dependencies
        ;;
    MINGW64_NT*|MSYS_NT*|CYGWIN_NT*)
        install_windows_dependencies
        ;;
    *)
        echo "Unknown OS detected - Exiting"
        exit 1
        ;;
esac