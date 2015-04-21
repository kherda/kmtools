<?php

error_reporting(E_ALL);
ini_set('display_errors', '1');
//error_reporting(E_ALL ^ E_NOTICE);

/* First test on the List Manager */
//require ('class.ecnlistmanager.php');

// $token = ''; // Your custom token should come from KM.
// $lm = new ListManager($token);
// echo "<pre>"; print_r($lm->GetFolders());
// die();

/* Second Test on UAD */
require ('class.uad.php');

// Editorial Token Example.
$token = '';

$uad = new UADManager($token);

$ret = $uad->GetSubscriber('kherda@sgcmail.com');
echo "<Pre>"; print_r($ret); die();

