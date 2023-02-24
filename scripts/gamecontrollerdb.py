import requests

# Fetch latest version of SDL gamecontrollerdb.txt
print('Fetching latest SDL_GameControllerDB...')

req = requests.get("https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt")
req.raise_for_status()

filename = "resources/config/gamecontrollerdb.txt"
with open(filename, "w") as controller_db:
    controller_db.write(req.text)

print(f"Wrote updates to {filename}.")
