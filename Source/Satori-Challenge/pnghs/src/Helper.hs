module Helper (warpList) where

import Control.Monad.Writer.Lazy


type List a = Writer [[a]] [a]

warper :: Int -> List a -> a -> List a
warper max w d = w >>= \r ->
        if length r == max
        then writer ([d], [r])
        else return $ r ++ pure d

warpList :: Int -> [a] -> [[a]]
warpList max d = let (x,y) = runWriter $ foldl (warper max) (pure []) d
                 in y ++ pure x


