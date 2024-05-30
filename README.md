## SKI Calculus Interpreter

This tree includes code for interpretation of extended version of SKI Calculus.

## Token definitions

| Token | Description |
| --- |-------------- | 
| `identifier` | Any sequence of characters which may contain lowercase alphabets, digits from 0 to 9 or an underscore. The sequence must start with a lowercase alphabet or an underscore. |
| `comment` | A sequence of characters which starts with `#` and ends with a newline. |

## Context Free Grammar

```
SKI        -> Defns Exprs                                                   => "ski";

Defns      -> (Defn ';')*                                                   => "defns";

Exprs      -> (Expr ';')*                                                   => "exprs";

Defn       -> 'def' <identifier> '=' Expr                                   => "defn";

Expr       -> SubExpr
           -> Expr SubExpr                                                  => "app";

SubExpr    -> <identifier>                                                  => "var";
           -> 'S'                                                           => "s";
           -> 'K'                                                           => "k";
           -> 'I'                                                           => "i";
           -> '(' Expr ')'
```

## Build

```
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```
