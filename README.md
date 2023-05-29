# APT-Attack-Simulation
A APT Attack Simulation for APT 29 &amp; Lockbit

# Authors

* Abdulrhman Mohammed => https://github.com/de3vil 
* Abdallah Mohammed   => https://github.com/abdallah-elsharif/
* Hossam Ehab         => https://github.com/0xHossam

# How does it works!

The attack contains main three stages

# First Stage:


* The attack begins with an email that contains
a specially crafted HTML page that includes a
malicious code. Upon opening the malicious
page, an ISO file is downloaded that contains
the malicious files.

* The malicious file performs two actions

1. Drops an ISO file (Second Stage)
2. Sends a request to a malicious SMB server
controlled by the attackers to steal the user's
NTLM Hash for lateral spread.

# Second Stage
* The ISO file is opened and contains a forged
lnk file that appears to be a PDF file from its
icon, but it actually points to an exe file. When the exe file is executed, it opens the PDF
file.

* Upon
execution of the malicious exe file, the PDF file is also executed. DLL and bin files are
loaded, a registry key is added for persistence, and the DLL runs
During this stage.

* At this point, the attacker enables UAC bypass, gains system administrator privileges,
hijack Windows Defender, and loads the malicious DLL instead of the original file. This
allows the attacker to invisibly direct the malicious functions on the victim's machine,
download more malware, and achieve system persistence.

# Third Stage

Third stage
* The DLL acts as a Loader to read the encrypted bin file and decrypt it in memory
and try to hide from detection and run.

# Disclaimer

Important Notice: This repository contains code and materials related to malware and other potentially harmful activities.

* Limitation of Liability

The code and information provided in this repository are intended for educational and research purposes only. We explicitly disclaim any responsibility or liability for any illegal use, damage, or negative consequences resulting from the use of this code. By accessing or utilizing the code and materials in this repository, you acknowledge and accept that you are solely responsible for your actions and any associated outcomes.

* Responsible Usage

We strongly discourage and condemn any form of illegal activities or malicious use of the code provided in this repository. It is essential to adhere to all applicable laws, regulations, and ethical guidelines when engaging with the information and code contained herein. We do not endorse, encourage, or support any unauthorized access, hacking, or malicious behavior.

* Release of Liability

By using this repository, you agree to release the repository owner(s) from any liability, claims, or legal actions arising from your use, misuse, or interpretation of the code or information presented here. The repository owner(s) cannot be held accountable for any damages, losses, or adverse consequences incurred by individuals or organizations using this repository.

* Ethical Considerations

We strongly urge users to prioritize ethical considerations and demonstrate responsible behavior when working with technology and code. Respecting the privacy, security, and rights of others is of utmost importance when utilizing any information or code found in this repository.

* Compliance with Laws

Users are expected to fully comply with all local, national, and international laws and regulations relevant to their jurisdiction. Any misuse or unauthorized use of the code or information provided in this repository is strictly prohibited and may result in legal consequences.

* Acknowledgment

We appreciate your understanding and cooperation in adhering to these terms. Please remember that technology and code should be used in a responsible, ethical, and positive manner.
