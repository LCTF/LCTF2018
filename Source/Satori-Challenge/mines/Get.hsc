{-# LANGUAGE CPP #-}
{-# LANGUAGE ForeignFunctionInterface #-}

module Get where

import Data.Char
import Data.Bits

foreign export ccall showflag :: IO ()

showflag :: IO ()
showflag = do
    putStr "LCTF{"
    putStr $ concat $ map show $ map (xor 0x45) $ map (xor 0x98) $ map (xor 0xE0) $ map ord "sadfRCHSks"
    putStrLn "}"
    return ()
