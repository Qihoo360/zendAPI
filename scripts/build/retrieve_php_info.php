#!/usr/bin/env php
<?php
/**
 * 这个脚本获取php版本号
 */

// function definitions

function print_usage()
{
    echo <<<USAGE
usage: retrieve_php_info.php includePath [--TypeOption]
the support options:
--PHP_VERSION_ID
--PHP_API_VERSION_NO
--ZEND_MODULE_API_NO
--ZEND_EXTENSION_API_NO
USAGE;
}

function print_php_version_id($includeDir)
{
    $headerFile = $includeDir . "/php/main/php_version.h";
    if (!file_exists($headerFile)) {
        echo "header file " . $headerFile . " not exists";
        exit(1);
    }
    $lines = file($headerFile);
    foreach ($lines as $line) {
        if (false !== strpos($line, "PHP_VERSION_ID")) {
            $parts = explode(" ", $line);
            echo trim($parts[2]);
            exit(0);
        }
    }
}

function print_php_api_no($includeDir)
{
    $headerFile = $includeDir . "/php/main/php.h";
    if (!file_exists($headerFile)) {
        echo "header file " . $headerFile . " not exists";
        exit(1);
    }
    $lines = file($headerFile);
    foreach ($lines as $line) {
        if (false !== strpos($line, "PHP_API_VERSION")) {
            $parts = explode(" ", $line);
            echo trim($parts[2]);
            exit(0);
        }
    }
}

function print_zend_module_api_no($includeDir)
{

    $headerFile = $includeDir . "/php/Zend/zend_modules.h";
    if (!file_exists($headerFile)) {
        echo "header file " . $headerFile . " not exists";
        exit(1);
    }
    $lines = file($headerFile);
    foreach ($lines as $line) {
        if (false !== strpos($line, "ZEND_MODULE_API_NO")) {
            $parts = preg_split("/\\s+?/", $line);
            echo trim($parts[2]);
            exit(0);
        }
    }
}

function print_zend_extension_api_no($includeDir)
{
    $headerFile = $includeDir . "/php/Zend/zend_extensions.h";
    if (!file_exists($headerFile)) {
        echo "header file " . $headerFile . " not exists";
        exit(1);
    }
    $lines = file($headerFile);
    foreach ($lines as $line) {
        if (false !== strpos($line, "ZEND_EXTENSION_API_NO")) {
            $parts = preg_split("/\\s+?/", $line);
            echo trim($parts[2]);
            exit(0);
        }
    }
}

$argc = $_SERVER["argc"];
$argv = $_SERVER["argv"];

// 我们只接受一个参数
if ($argc < 3) {
    print_usage();
    exit(1);
}

$includeDir = $argv[1];
$includeDir = rtrim($includeDir, "/");
if (!file_exists($includeDir)) {
    echo "include directory not exists";
    exit(1);
}

$infoType = $argv[2];

switch ($infoType)
{
    case "--PHP_VERSION_ID":
        print_php_version_id($includeDir);
        break;
    case "--PHP_API_VERSION_NO":
        print_php_api_no($includeDir);
        break;
    case "--ZEND_MODULE_API_NO":
        print_zend_module_api_no($includeDir);
        break;
    case "--ZEND_EXTENSION_API_NO":
        print_zend_extension_api_no($includeDir);
        break;
    default:
        echo "unsupport OptionType ".$infoType;
        exit(1);
        break;
}
exit(0);
