import module namespace m = 'xqueryzorba.org/test/xqddf/ic' at 'ic.xqlib';
import module namespace ddl = "http://zorba.io/modules/store/static/collections/ddl";
import module namespace dml = "http://zorba.io/modules/store/static/collections/dml";
import module namespace ic_ddl = "http://zorba.io/modules/store/static/integrity-constraints/ddl";



ddl:create($m:empc);  

ic_ddl:activate(xs:QName("m:ic_uniqueKey"));


(: employees :)
dml:insert($m:empc, 
   <emp>
     <id>1</id>
   </emp>
  )
,
dml:insert($m:empc, 
   <emp>
     <!-- id element is missing, must break IC -->
   </emp>
  )
,
dml:insert($m:empc, 
   <emp>
     <id>3</id>
   </emp>
  );

fn:data(dml:collection($m:empc)/id)
,
let $ids := dml:collection($m:empc)/id 
return fn:count( $ids ) eq fn:count( fn:distinct-values( $ids ) );
