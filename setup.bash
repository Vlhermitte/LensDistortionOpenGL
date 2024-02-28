#!/usr/bin/env bash

# bash script to install the necessary dependencies for the project

# windows
if (uname -a | grep -q "MINGW64_NT"); then
    echo "Windows detected - Nothing to do"
fi # end windows

# linux
if (uname -a | grep -q "Linux"); then
    echo "Linux detected - Installing dependencies"
    # install GLFW3
    sudo apt-get install libglfw3
    # install GLM
    sudo apt-get install libglm-dev
    # install GLEW
    sudo apt-get install libglew-dev
    # install GL
    sudo apt-get install libgl-dev
    # install Assimp
    sudo apt-get install libassimp-dev
fi # end linux

# mac
if (uname -a | grep -q "Darwin"); then
    echo "Mac detected - Installing dependencies"
    # check if brew is installed
    if ! [ -x "$(command -v brew)" ]; then
        echo "Error: brew is not installed. Would you like to install it? (y/n)"
        read -r response
        # if y or Y
        if [[ "$response" =~ ^([yY])$ ]]; then
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        else
            echo "Exiting"
            exit 1
        fi
    fi
    # install GLFW3
    brew install glfw
    # install GLM
    brew install glm
    # install GLEW
    brew install glew
    # install GL
    brew install gl
    # install Assimp
    brew install assimp
fi # end mac