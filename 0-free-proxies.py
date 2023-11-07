import requests
import pandas as pd

# URL to fetch free proxies
FREE_PROXIES_URL = "https://free-proxy-list.net/"

if __name__ == "__main__":
    # Get proxies into a pandas DataFrame
    free_proxies = requests.get(FREE_PROXIES_URL, timeout=5)
    proxy_list = pd.read_html(free_proxies.text)[0]

    # Filter proxies with "https" equal to "yes"
    https_proxies = proxy_list[proxy_list["Https"] == "yes"]

    # Create a text file to write IP addresses and ports
    with open("free-proxies.txt", "w") as file:
        # Iterate through DataFrame rows and write IP addresses and ports to the text file
        for _, row in https_proxies.iterrows():
            ip_address = row["IP Address"]
            port = row["Port"]
            file.write(f"{ip_address}:{port}\n")
