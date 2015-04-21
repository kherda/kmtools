<?php
/**
 * UAD Manager
 *
 * The UAD Manager is a set of API calls with will extract subscriber based
 * information from the Master File (MAF) in Knowledge Marketing.
 * 
 * @package    ECN Suite
 * @author     Kevin Herda <kherda@sgcmail.com.com>
 * @version    Release: 1.2
 */

require ('class.ecncommunicator.php');

/**
 *
 */
class UADManager extends Communicator {

  /**
   * The token is the authentication key used to validate the API request.
   * @var String
   */
  protected $token;

  /**
   * Portal
   *
   * The portal will either be ECN or UAD.
   * @var  String
   */
  protected $portal;

  /**
   * The keyname is slightly different accross a few API calls, and definitely different from ECN to UAD.
   * @var string
   */
  protected $keyname = 'accessKey';

  /**
   * Manager for use in the Communicator
   * @var string
   */
  protected $manager = NULL;

  /**
   * Extension for ECN API Calls
   *
   * NOTE:  Null for UAD.
   */
  protected $ext = NULL;

  /**
   * Addional slash for UAD Param
   * @var string
   */
  protected $slash = '/';

  /**
   * Extraction
   *
   * The extraction is how we will render the output of the API Call.
   * @var String.
   */
  protected $extraction;

  /**
   * Construct the protected elements of the class.
   * @param String $token
   */
  public function __construct($token, $extraction = 'json') {

    $this->token = $token;
    $this->portal = 'UAD';
    $this->extraction = $extraction;
  }

  /**
   * Get Subscriber
   * @param String $email
   *   A valid email addresses is required to query the database.
   */
  public function GetSubscriber($email) {

    $params = "&email={$email}";

    return parent::execute(__FUNCTION__, $params);
  }

}
