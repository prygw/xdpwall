#!/bin/bash
#run ip a and check interface
INTERFACE="enp2s0"
XDP_OBJ="../bin/xdp_checker.o"
XDP_SEC="xdp"
LOADER="../bin/load"
LISTER="../bin/list"

if [ $# -eq 0 ]; then
    echo "Usage: $0 {enable|disable|allow|list|clear}"
    exit 1
fi

COMMAND=$1

case "$COMMAND" in
    enable)
        echo "Attaching XDP program to $INTERFACE..."
        sudo ip link set dev "$INTERFACE" xdp obj "$XDP_OBJ" sec "$XDP_SEC"
        ;;
        
    disable)
        echo "Detaching XDP program from $INTERFACE..."
        sudo ip link set dev "$INTERFACE" xdp off
        ;;
        
    allow)
        if [ -z "$2" ]; then
            echo "Error: You must provide a list of ports inside quotes."
            echo "Example: $0 allow \"80 1234 99 67\""
            exit 1
        fi
        
        PORTS=$2
        echo "Sending ports [$PORTS] to loader..."
        echo "$PORTS" | sudo "$LOADER"
        ;;

    list)
        echo "Listing allowed ports..."
        if [ -x "$LISTER" ]; then
            sudo "$LISTER"
        else
            echo "Error: $LISTER not found or not executable."
        fi
        ;;

    clear)
        echo "Clearing all allowed ports..."
        echo "" | sudo "$LOADER"
        ;;
        
    *)
        echo "Invalid argument: $COMMAND"
        echo "Usage: $0 {enable|disable|allow \"ports\"|list|clear}"
        exit 1
        ;;
esac
