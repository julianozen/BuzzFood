BuzzFood
========

Buzzfood is a smart microwave designed to make cooking anything as simple as making popcorn. Microwaves, despite their simplicity are almost never used as effectively as they can be. People, rarely adjust the temperature setting, risk burning themselves on over-powered microwaves, and accept the subpar results they produce.

We believe that a smart microwave can be a revolution in making food by allowing complex cooking without any hassle. Users simply scan the barcode of their product, follow the step by step instructions, and put their food in to cook. No tiny labels, no frozen insides.

Long term, we believe this product can inform the food placed inside it. We can offer more complex settings than simply low or high, and can offer user the ability to pick between “fast” or “better”. Simple machine learning and crowdsourcing can make sure your burrito is cooked through every time.

Our current hack has a diverse stack. An iPhone app scans a barcode, and pulls the instruction data from the server. The server then interacts with the microwave, so that the user doesn’t need to manually input any data. The server connects to a Spark Core, a microcontroller that we programmed to send input to the microwave. We reprogrammed the microcontroller of the microwave to overwrite the use of the touchpad and listen to our inputs instead.
