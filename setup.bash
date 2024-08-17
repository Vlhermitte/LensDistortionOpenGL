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
        # Define the local installation path for Chocolatey
        LOCAL_CHOCOLATEY_PATH="$HOME/choco-local/bin"

        # Check if choco is installed locally
        if ! [ -x "$LOCAL_CHOCOLATEY_PATH/choco.exe" ]; then
            echo "Chocolatey is not installed. Installing locally..."

            # Create a local bin directory if it doesn't exist
            mkdir -p "$LOCAL_CHOCOLATEY_PATH"

            # Download and install Chocolatey in the local directory
            powershell -NoProfile -ExecutionPolicy Bypass -Command "Set-ExecutionPolicy Bypass -Scope Process; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1')); Install-Chocolatey.ps1 -InstallDir '$LOCAL_CHOCOLATEY_PATH'"

            # Ensure the local Chocolatey path is in the PATH
            export PATH="$LOCAL_CHOCOLATEY_PATH:$PATH"
        fi

        # Use local Chocolatey to install dependencies
        "$LOCAL_CHOCOLATEY_PATH/choco.exe" install cmake --version=3.27 -y
        "$LOCAL_CHOCOLATEY_PATH/choco.exe" install make -y
        "$LOCAL_CHOCOLATEY_PATH/choco.exe" install glfw -y
        "$LOCAL_CHOCOLATEY_PATH/choco.exe" install glm -y
        "$LOCAL_CHOCOLATEY_PATH/choco.exe" install glew -y
        "$LOCAL_CHOCOLATEY_PATH/choco.exe" install assimp -y

    # Python
    ""$LOCAL_CHOCOLATEY_PATH/choco.exe"" install python3 -y
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