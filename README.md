# Knowledge Marketing Tools

The KM Tools open source code is meant for those looking to link their website with the ECN / UAD.  These tools provide you with the ability to easily access all API calls within the List Manager, thus allowing you to manage and maintain your groups and subscribers from the website.

1. Drupal 7 - The Drupal dir contains 3 plugins you can use on your Drupal 7 website.

  - KMSUITE:  The core module that contains optional domain tracking and taxonomy tracking.  The domain tracking option required a domain tracking key (found at the group level in the ECN).  The taxonomy tracking will leverage vocabularies in Drupal with optional prefix, case transformation and term exclusion.

  - KMSUBSCRIPTION:  This is the main module that communicates between your website and the ECN.  Enabling this module will create a subscription block, and also a custom page.  Both the block and the page are customizable from the groups you want to show, or the fields you want to have in the form.  NOTE:  You can organize the fields and groups, and give each group an alias.  Finally, the administration section of this module allows you to add folders and groups (incase you need to do this outside of the ECN).

  NOTE:  When adding fields, you have the option to add custom UDF's (user defined fields).  If you choose to add this, you will have to make sure the field you have created is created for the group in the ECN.  Creating the field in the website will not tie it to groups in the ECN (as we cannot add 1 to many, nor would we want too).

  - KMOVERLAY:  This additional module allows you to display a form in an overlay in Drupal 7 (using ctools).  This form is customizable, and allows additional administration options (such as redirecting the user after a sucessful entry).

  - RERPOTING:  During the installation of the KMSUBSCRIBTION module, a table in the database will be created that will allow us to track who has subscribed in the website. Each plugin details a different location of where someone signed up (overlay, page, block, etc).  You can run simple reporting by visiting this page (admin/reports/kmsubscription)

2. PHP API:  If you would like to use an alternate CMS, or would like to use custom code to connect to the ECN, we have provided an optional PHP Wrapper for you.  This code contains all the List Manager API calls and the Communicatior for you to use when executing the call.  Simply visit the index.php page for an example.

  NOTE:  You need a KM Token to demonstrate any API method.  Contact Knowledge Marketing for the token.
