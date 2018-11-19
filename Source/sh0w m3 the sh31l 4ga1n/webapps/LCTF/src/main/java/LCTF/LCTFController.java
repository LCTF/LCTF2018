package LCTF;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import static LCTF.xmlToMap.*;
import javax.servlet.http.HttpServletRequest;
import java.util.Map;
import java.util.regex.Pattern;


@RequestMapping(value = "/api")
@Controller
public class LCTFController {

    @CrossOrigin(origins = "http://212.64.74.153")
    @RequestMapping(method = RequestMethod.POST,consumes = "application/json")
    @ResponseBody
    public Ret JsonRe(@RequestBody Items items) throws Exception {
        Ret rets = new Ret();
        if(Pattern.matches("(?i)(.|\\s)*(<|>|!|%|SYSTEM|ENTITY)(.|\\s)*",items.getContent()) ||Pattern.matches("(.|\\s)*(<|>|!|%|SYSTEM|ENTITY)(.|\\s)*",items.getName())){

            rets.setStatus("Error");
            rets.setMessage("Hacker! Hacker! Hacker!");
            return rets;
        }else{
            rets.setStatus("OK");
            rets.setMessage("Thank you for your comment!");
            return rets;
        }
    }

    @CrossOrigin(origins = "http://212.64.74.153")
    @RequestMapping(method = RequestMethod.POST,consumes = "application/xml")
    @ResponseBody
    public String XmlRe(@RequestBody String data, HttpServletRequest request) throws Exception {

        if(Pattern.matches("(?i)(.|\\s)*(file|ftp|gopher|CDATA|read_secret|logs|log|conf|etc|session|proc|root|history|\\.\\.|data|class|bash|viminfo)(.|\\s)*",data)){
            return "{\"status\":\"Error\",\"message\":\"Hacker! Hacker! Hacker!\"}";
        }else{
            Map<String,String> test = xmlToMap(data);
            return "{\"status\":\"Error\",\"message\":\""+ test +"\"}";
        }

    }

}