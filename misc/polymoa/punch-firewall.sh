#!/bin/bash

set -e

# Accept connection to flecs-agent on polynesias.
iptables -I INPUT 1 -p tcp --dport 10000 -m state --state NEW -j ACCEPT
iptables -I INPUT 1 -p tcp --dport 10001 -m state --state NEW -j ACCEPT
iptables -I INPUT 1 -p tcp --dport 10002 -m state --state NEW -j ACCEPT
iptables -I INPUT 1 -p tcp --dport 10010 -m state --state NEW -j ACCEPT
