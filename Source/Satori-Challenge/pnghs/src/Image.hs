module Image (encodePicture) where

import Data.Bits
import System.IO
import Codec.Picture
import Codec.Picture.Types

import Helper

type Px = (Int, Int)

bitX :: Int -> Int -> Int
bitX d n = shiftR d n .&. 0x01

-- Word8 => Int
-- RGB LSB. Use R and G
splitData :: [Int] -> [Px]
splitData d = func <$> d <*> [6, 4, 2, 0]
    where
        func x y = (bitX x $ y + 1, bitX x y)

data PxMatrix = PxMatrix {
    pxLen :: Int,
    pxEnd :: Int,
    pxData :: [[Px]]
}

matrixData :: Image PixelRGB8 -> [Px] -> PxMatrix
matrixData img pxs = let d = warpList (imageWidth img) pxs
                     in PxMatrix (length d - 1) (length (last d) - 1) d

matrixAt :: PxMatrix -> Int -> Int -> Px
matrixAt pm x y
    | y > pxLen pm = (0,0)
    | (y == pxLen pm) && (x > pxEnd pm) = (0,0)
    | otherwise = (pxData pm) !! y !! x


mixImage :: Image PixelRGB8 -> PxMatrix -> Image PixelRGB8
mixImage img pm = pixelMapXY func img
    where
        up n px = fromIntegral (xor n $ fromIntegral px)
        func x y (PixelRGB8 r g b) = let (r',g') = matrixAt pm x y
                                     in PixelRGB8 (up r' r) (up g' g) b

encodePicture :: FilePath -> FilePath -> [Int] -> IO ()
encodePicture ifp ofp d = readImage ifp >>= \e ->
    case e of
        Left msg -> putStrLn msg
        Right dimg -> let img = convertRGB8 dimg in
            writePng ofp $ mixImage img $
                matrixData img $ splitData d


