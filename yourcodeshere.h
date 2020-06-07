//Rename this file to "wificodes.h" and add your personal details below:
char ssid[] = "YOUR SSID HERE";
char pass[] = "YOUR PASSWORD HERE";
int keyIndex = 0; //<-- Only required for WEP encryption (see README.md usage part 4).
//Requires the key for the legacy api. It is simpler and better suited while it still runs.
//Keys can be obtained from  https://osu.ppy.sh/p/api/
String apikey = "YOUR API KEY HERE";
//Query string appended to osu.ppy.sh, right before the apikey.
String query = "/api/get_user?u=YOUR USERNAME HERE";