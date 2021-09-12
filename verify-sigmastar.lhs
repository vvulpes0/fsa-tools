> module Main where

> import System.Exit (ExitCode(..), exitWith)
> import qualified Data.Set as Set

> import LTK

> main = exitWith =<< fmap (int . isSS . normalize . from ATT) getContents
>     where int x = if x then ExitSuccess else ExitFailure 1

> isSS = isNull . complement . unset
> unset f = f {sigma = unsymbols . Set.map edgeLabel $ transitions f}
