# Knowledge Marketing Tools

The KM Tools open source code is meant for those looking to link their website with the ECN / UAD.  These tools provide you with the ability to easily access all API calls within the List Manager, thus allowing you to manage and maintain your groups and subscribers from the website.

1. Drupal 7 - The Drupal dir contains 4 plugins you can use on your Drupal 7 website.

  - KMSUITE:  The core module that contains optional domain tracking and taxonomy tracking.  The domain tracking option requires a domain tracking key (found at the group level in the ECN).  The taxonomy tracking will leverage vocabularies in Drupal with optional prefix, case transformation and term exclusion.

  Update:  Removal of fixed HTTP:// for loading the Tracker.js file.  I have changed this to be protocol-relative, for those who have a site under SSL.

  - KMSUBSCRIPTION:  This is the main module that communicates between your website and the ECN.  Enabling this module will create a subscription block, and also a custom page (subscription-management).  Both the block and the page are customizable from the groups you want to show, or the fields you want to have in the form.  NOTE:  You can organize the fields and groups, and give each group an alias.  Finally, the administration section of this module allows you to add folders and groups (incase you need to do this outside of the ECN).

  Update:  We have separated the fields and page settings, making it easier to manage the config.  We also added basic API tools for easier management, such as adding folders and groups.

  NOTE:  When adding fields, you have the option to add custom UDF's (user defined fields).  If you choose to add this, you will have to make sure the field you have created is created for the group in the ECN.  Creating the field in the website will not tie it to groups in the ECN (as we cannot add 1 to many, nor would we want too).

    - REPORTING:  During the installation of the KMSUBSCRIPTION module, a table in the database will be created that will allow us to track who has subscribed in the website. Each plugin details a different location of where someone signed up (overlay, page, block, etc).  You can run simple reporting by visiting this page (admin/reports/kmsubscription)

  - KMOVERLAY:  This additional module allows you to display a form in an overlay in Drupal 7 (using ctools).  This form is customizable, and allows additional administration options (such as redirecting the user after a sucessful entry).

  Requirement:  You must have ctools on your site for this to work.  We leverage the ctools modules for the overlay, which you can customize however you would like.

  - KMUAD:  A new module that allows connecting directly to the UAD database for consensus lookups (bypassing the ECN altogether).  This module contains a very useful configuration interface, which will allow you to pull the UAD schema out, and made custom tokens off of those values accordingly (no need to go to KM for this).  Once the schema is created, new users with the ECNEmailAddress cookie will save the dimentions values as a new cookie (saved for 2 weeks).  Additionally, there is an easy to use function that will build a query with the dimensions you would like (useful for 3rd party tools, like ad units): kmuad_demographics_query().  The end goal with a situation like that is to pass user dimension data to the ad units for granular control (ex. ad placement: website -> page -> taxonomy term -> job title&job function)

2. PHP API:  If you would like to use an alternate CMS, or would like to use custom code to connect to the ECN, we have provided an optional PHP Wrapper for you.  This code contains all the List Manager API calls and the Communicatior for you to use when executing the call.  Simply visit the index.php page for an example.

  NOTE:  You need a KM Token to demonstrate any API method.  Contact Knowledge Marketing for the token.

  Update:  The UADManager will communicate to the UAD, still using the connector for the cURL connection.  Useful for easy debugging.
