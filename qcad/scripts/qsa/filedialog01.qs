/**
 * Author: Andrew Mustun
 * $Id: filedialog01.qs 921 2004-12-19 00:31:22Z andrew $
 *
 * Simple demo of an open file dialog.
 */
function main() {
	var file = FileDialog.getOpenFileName("*.txt");
	debug("Selected file is: " + file);
}

