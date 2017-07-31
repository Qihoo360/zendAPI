<?php
/**
 * zendAPI project test entry
 *
 * @author ZZU_SOFTBOY <cntysoft@163.com>
 * @copyright  Copyright (c) 2010-2011 Ty Technologies China Inc. <http://www.zendapi.org>
 * @license    http://www.zendapi.org/LICENSE.txt
 */
define("ZAPI_TEST_ROOT_DIR", __DIR__);
define("ZAPI_TEST_BEGIN_MARKER", "--TEST--");
define("ZAPI_TEST_FILE_MARKER", "--FILE--");
define("ZAPI_TEST_EXPECT_MARKER", "--EXPECT--");

$totalTests = 0;
$passedTests = 0;
$failedTests = 0;
$failedTestNames = [];

function run_test($filename)
{
   global $passedTests;
   global $failedTests;
   try {
      if (!file_exists($filename)) {
         throw new \Exception(" test file " . $filename . " not exist\n");
      } else {
         $testMeta = retrieve_test_meta($filename);
         echo "run test: " . $testMeta["msg"];
         ob_start();
         eval($testMeta["code"]);
         $result = ob_get_clean();
         echo " -- ";
         if ($testMeta["expect"] == $result) {
            $passedTests++;
            echo render_green_msg("passed\n");
         } else {
            echo render_red_msg("failed\n");
            $failedTests++;
         }
      }
   }catch (\Exception $exception) {
      $failedTests++;
      echo render_red_msg("error:") . $exception->getMessage();
   }
}

function retrieve_test_meta($filename)
{
   $content = file_get_contents($filename);
   $msgMarkPos = strpos($content, ZAPI_TEST_BEGIN_MARKER);
   $codeMarkPos = strpos($content, ZAPI_TEST_FILE_MARKER);
   $expectMarkPos = strpos($content, ZAPI_TEST_EXPECT_MARKER);
   if (false === $msgMarkPos || false === $codeMarkPos || false === $expectMarkPos) {
      throw new \Exception(" $filename test file format error");
   }
   $meta = array();
   $meta["msg"] = trim(substr($content, $msgMarkPos + strlen(ZAPI_TEST_BEGIN_MARKER), $codeMarkPos - $msgMarkPos - strlen(ZAPI_TEST_BEGIN_MARKER)));
   $meta["code"] = trim(substr($content, $codeMarkPos + strlen(ZAPI_TEST_FILE_MARKER), $expectMarkPos - $codeMarkPos - strlen(ZAPI_TEST_FILE_MARKER)));
   $meta["code"] = substr($meta["code"], 5, -2);
   $meta["expect"] = trim(substr($content, $expectMarkPos + strlen(ZAPI_TEST_EXPECT_MARKER)));
   return $meta;
}

function main($argc, $argv)
{
   global $totalTests;
   $tests = array();
   if ($argc >= 2) {
      array_shift($argv);
      while($argv) {
         $tests[] = array_shift($argv);
      }
   } else {
      // 递归探测
      $directory = new \RecursiveDirectoryIterator(ZAPI_TEST_ROOT_DIR);
      $iterator = new RecursiveIteratorIterator($directory);
      foreach ($iterator as $entry) {
         if ($entry->isFile()) {
            if ("phpt" == $entry->getExtension()) {
               $tests[] = str_replace(ZAPI_TEST_ROOT_DIR."/", "", $entry->getPathname());
            }
         }
      }
   }
   $totalTests = count($tests);
   print_header();
   foreach ($tests as $testfilename) {
      run_test($testfilename);
   }
   print_summary();
}

function render_red_msg($msg)
{
   return "\x1B[01;31m".$msg."\x1B[00m";
}

function render_green_msg($msg)
{
   return "\x1B[01;32m".$msg."\x1B[00m";
}

function print_header()
{
   echo "\n================== begin run phpt tests ==================\n\n";
}

function print_summary()
{
   global $totalTests;
   global $passedTests;
   global $failedTests;
   echo "\n================== summary ===============================\n\n";
   echo "total tests : $totalTests\n";
   echo "passed tests : $passedTests\n";
   echo "failed tests : $failedTests\n";
   echo "\n================== summary ===============================\n\n";
}

// run tests
main($_SERVER["argc"], $_SERVER["argv"]);