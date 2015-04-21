<?php
/**
 * ECN Communicator
 *
 * The ECN Communicator is the common file that does the leg work for making API Calls
 * work between various Knowledge Marketing Libraries.
 *
 * Note:  Inherited objects are required for this class to operate.
 *
 * @package    ECN Suite
 * @author     Kevin Herda <kherda@sgcmail.com.com>
 * @version    Release: 1.2
 */

class Communicator {

  /**
   * ECN URL
   * @var String
   */
  protected $ecn_url = 'http://webservices.ecn5.com/';

  /**
   * UAD URL
   * @var String
   */
  protected $uad_url = 'https://uadservices.kmpsgroup.com/UADService/';

  /**
   * Executor for the ECN / UAD API Calls.
   * @param  String $request
   *   This must be the function we are return, that is case senstive from KM.
   * @param  String $params
   *   Params are constructed in the inherited class object.
   * @return mixed
   *   We are either returning an array that we extract from the XML response, or the JSON object from the UAD.
   */
  public function execute($request, $params){

    $this->portal == 'ECN' ? $portal_url = $this->ecn_url : $portal_url = $this->uad_url;

    $url = (string) $portal_url . $this->manager . $this->ext . $request . $this->slash . '?' . $this->keyname . '=' . $this->token . $params;

    $ch = curl_init($url);

    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    $resp = curl_exec($ch);

    curl_close($ch);

    if ($this->extraction == 'json'){
      $xml = json_decode($resp);
    }
    elseif ($this->extraction == 'array') {

      $xmlObject = new SimpleXMLElement($resp);
      $xml = simplexml_load_string($xmlObject);
      $xml->registerXPathNamespace('soap', 'http://schemas.xmlsoap.org/soap/envelope/');
    }

    return $xml;
  }

}
