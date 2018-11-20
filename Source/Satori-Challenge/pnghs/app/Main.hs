module Main where

import Data.Char
import System.IO
import System.Environment

import Crypto
import Image


main :: IO ()
main = getArgs >>= parse
--main = putStrLn $ show $ encryptoData $ map ord plainText

parse :: [[Char]] -> IO ()
parse [fin, fout, fdata] = do
    plaintext <- readFile fdata
    encodePicture fin fout $ encryptoData $ map ord plaintext

parse arg = putStrLn "Usage: pnghs input-img output-img data-file"


