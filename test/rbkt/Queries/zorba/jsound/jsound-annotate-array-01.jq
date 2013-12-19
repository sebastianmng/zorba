import module namespace jsd = "http://jsound.io/modules/jsound"; 

let $jsd :=
  {
    "$namespace" : "http://www.example.com/my-schema",
    "$types" : [
      {
        "$kind" : "array",
        "$name" : "foo",
        "$content" : [ "integer" ]
      }
    ]
  }

let $instance := "bar" (: must be array :)

return jsd:annotate( $jsd, "foo", $instance )

(: vim:set syntax=xquery et sw=2 ts=2: :)
