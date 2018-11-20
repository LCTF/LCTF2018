module Crypto (encryptoData) where

import Data.Bits
import Data.Char
import Control.Monad.State.Lazy
import Control.Monad.Writer.Lazy

import Helper

{- Static Vaule -}
initIV :: [Int]
initIV = map ord "701cc9a49e30d35bebb81fbafc3c9bea"

initKey :: [Int]
initKey = map ord "KomeijiSatori"

blockLen :: Int
blockLen = 32

paddingData :: Int
paddingData = 5

{- Helper Function -}

paddingX :: Int -> a -> [a] -> [a]
paddingX n c d = d ++ replicate n c

{- Encrypto -}

type Calc = State [Int] [Int]

xorKey :: [Int] -> Int -> Calc
xorKey key n = gets $ \iv ->
    map (xor n) $ zipWith xor iv $
    take (length iv) $ cycle key

calcData :: Calc
calcData = do
    y <- xorKey initKey 0x39
    return $ map (xor 0xFF) y

encryption :: [Int] -> [Int]
encryption d = evalState calcData d

{- Block Crypto -}

type DataBlock = Writer [Int] [Int]

initBlock :: [Int] -> DataBlock
initBlock iv = return iv

-- OFB Mode
encodeBlock :: ([Int] -> [Int]) -> DataBlock -> [Int] -> DataBlock
encodeBlock f db msg = db >>= (\iv -> writer (f iv, zipWith xor msg $ f iv ))

{- IO -}

foldData :: [[Int]] -> DataBlock
foldData list = foldl (encodeBlock encryption) (initBlock initIV) list

setData :: [Int] -> [[Int]]
setData d = warpList blockLen $
    paddingX ((-) blockLen $ length d `mod` blockLen) paddingData d

encryptoData :: [Int] -> [Int]
encryptoData d = execWriter $ foldData $ setData d


