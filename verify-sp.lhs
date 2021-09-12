> module Main where

> import System.Exit (ExitCode(..), exitWith)

> import LTK

> main = exitWith =<< fmap (int . isSP . normalize . from ATT) getContents
>     where int x = if x then ExitSuccess else ExitFailure 1
