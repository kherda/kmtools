
/**
 * @file
 *
 * The Javascript below is checking multiple conditions to display the overlay or not.  Due to
 * caching, we need to run the conditionals on the client side to determine if the popup
 * should show or not.
 *
 * NOTE:  SESS prefix on cookies is required to work on Varnish cached website.
 */

(function($) {
  Drupal.behaviors.kmoverlay = {
    attach: function (context, settings) {

      var path = Drupal.settings.kmoverlay.path;
      var status = Drupal.settings.kmoverlay.status;
      var developer = Drupal.settings.kmoverlay.developer;
      var interval = Drupal.settings.kmoverlay.interval;
      var delay = Drupal.settings.kmoverlay.delay;
      var pester = Drupal.settings.kmoverlay.pester;
      var inherited_pagectr = Drupal.settings.kmoverlay.pagectr;
      var force = 0;

      !$.cookie('SESSkmpagectr') ? pagectr = 0 : pagectr = $.cookie('SESSkmpagectr');

      // If the pester option is on, we handle things a bit different with the storage of cookies.
      if (pester == 1) {
        if (delay == 0) {
          force = 1;
        }
        else if (delay > 0) {
          if (pagectr % (delay + 1) == 0) {
            force = 1;
          }
        }
      }

      if (path == 0 && status == 1 && $.cookie('SESSkmuserclosed') != 1 && !$.cookie('ECNEmailAddress') && pagectr == delay  || developer == 1 || force == 1 && path == 0 && $.cookie('SESSkmuserclosed') != 1) {
        if (inherited_pagectr == pagectr) {
          $('.ctools-modal-ctools-overlay-style', context).click();
        }
      }

      // Set cookie if the user closed out of the overlay.
      if ($('#modalContent .popups-close').click(function(){
        if (developer == 0 && status == 1 && pester == 0){
          $.cookie("SESSkmuserclosed", 1, { expires: interval });
        }
      }));

      // Tally the page ctr to use against the delay (if applicable).  Expire it in 1 day:  1 day incase they hit the X key and not close
      // so we can hit them up with it again.
      if (path == 0 && !$.cookie('SESSkmuserclosed') && pagectr <= delay && developer == 0 && status == 1 || pester == 1 && path == 0 && $.cookie('SESSkmuserclosed') != 1) {

        // This conditional is to prevent multiple loads of this behavior from tampering with the pagectr.
        if (inherited_pagectr == pagectr) {
          $.cookie("SESSkmpagectr", ++pagectr, { expires: interval });
        }
      }
    }
  };

})(jQuery);

Drupal.theme.prototype.CToolsOverlayModal = function () {
  var html = '';

  html += '<div id="ctools-modal" class="popups-box">';
  html += '  <div class="ctools-modal-content ctools-sample-modal-content">';
  html += '      <div class="modal-header popups-title clearfix">';
  html += '        <span id="modal-title" class="modal-title"></span>';
  html += '        <span class="popups-close close">' + Drupal.CTools.Modal.currentSettings.closeImage + '</span>';
  html += '      </div>';
  html += '     <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body"></div></div>';
  html += '  </div>';
  html += '</div>';

  return html;
}
