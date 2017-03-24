import requests

def sendMail():
	var = requests.post(
		"https://api.mailgun.net/v3/apikey.mailgun.org/messages",
		auth=("api", "apikey"),
		data={"from": "Mailgun Sandbox <postmaster@sandbox.mailgun.org>",
		      "to": "atte <send.to@email.com>",
		      "subject": "Ovenavauspyyntö",
		      "text": "Ovea on pyydetty avaamaan järjestelmään kirjatulla kortilla, hyväksy ovenavaus http://api.botbook.com/add/apikey/?x=2"})
	print(var)