case "$(echo -e "Poweroff \nReboot ﰇ\nSuspend ⏾\nCancel ﰸ" | dmenu)" in
    "Poweroff ") loginctl poweroff ;;
    "Reboot ﰇ") loginctl reboot ;;
    "Suspend ⏾") loginctl suspend ;;
    "Cancel ﰸ") exit ;;
esac