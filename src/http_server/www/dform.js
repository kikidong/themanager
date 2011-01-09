// JavaScript Document
var items = 1;
function AddItem() {
   if (!document.getElementById) return;
  
   div = document.getElementById("items");
   button = document.getElementById("inputwriteroot");
   items++;
   
   newitem = "<b ><select name=\"lessname\"><optgroup label=\"German Cars\" name=\"opt" + items + "\">";
   newitem += "<option value=\"mercedes\">Mercedes</option>";
   newitem += "<option value=\"audi\">Audi</option>";
   newitem += "</optgroup></select> </b>  ";
   
   newitem += "<b>";
   newitem += "<input type=\"text\" name=\"score" + items + "\"/>";
   newitem += "</b>";   
   newitem += "<br />";
   
   newnode = document.createElement("span");
   newnode.innerHTML = newitem;
   div.insertBefore(newnode,button);
}
function Show(a) {
   if (!document.getElementById) return;
   obj = document.getElementById("input_student_info");
   if (a)
    obj.style.display = "block";
   else
    obj.style.display = "none";
}

