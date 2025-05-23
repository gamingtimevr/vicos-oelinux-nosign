#!/bin/bash

# Wire @kercre123 - simple script to switch bootctl

CMDLINE="$(cat /proc/cmdline)"

if [[ ${CMDLINE} == *"androidboot.slot_suffix=_b"* ]]; then
	bootctl b set_active a
	echo "Current slot is b, setting a active."
else
	bootctl a set_active b
	echo "Current slot is a, setting b active."
fi

sync
echo "Rebooting..."
reboot
