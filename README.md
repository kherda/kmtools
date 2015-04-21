# Knowledge Marketing Tools

The KM Tools open source code is meant for those looking to link their website with the ECN / UAD.  These tools provide you with the ability to easily access all API calls within the List Manager, thus allowing you to manage and maintain your groups and subscribers from the website.

1. Drupal 7 - The Drupal dir contains 3 plugins you can use on your Drupal 7 website.
  - KMSUITE:  The core module that enabled taxonomy tracking, domain tracking and also is the placeholder for the KM Token.
  - KMSUBSCRIPTION:  This module enabled field, groups, adds a block and a page.  You can also manage and maintain your folders and groups from this module.  In short, enabling this gives you a subscription page and a block.  Both are completely customizable (from groups to fields).
  - KMOVERLAY:  This additional module allows you to display a form in an overlay in Drupal 7 (using ctools).  This form is customizable, and allows additional administration options (such as redirecting the user after a sucessful entry).

2. PHP API:  If you plan on implementing your own version of the KM Tools, you can use the API wrapper by itself.  To easily use this, simply reference the index.php file for a quick example.  Your only requirement on using this module is a token delivered from Knowledge Marketing.
