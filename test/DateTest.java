/**
 * @file   DateTest.java
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2017 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

/** Assumes an existing graph with some date property that can be
 *  specified as the second argument, since
 *  we do not provide a set_property function for date in the
 *  Java bindings API.
 *  Currently tested with propertygraph generated by propertytest
 *  using property name "id7" to test Indian time zone, count = 4 AND
 *  with emailindexgraph created using email.gson using property
 *  "DeliveryTime" and count = 13
 */
import java.util.Calendar;
import java.util.Date;
import java.util.TimeZone;
import jarvis.*;

public class DateTest {
    public static void main(String[] args)
    {
        if (args.length != 3) {
            System.out.println("Usage: DateTest <graph> <propertyname> <expected count>");
            System.out.println("Look for dates after July 1 2014 17:00 GMT.");
            System.exit(1);
        }
        String graph_name = args[0];
        String prop_name = args[1];
        int expected = Integer.parseInt(args[2]);

        // Zone abbreviations are not recommended. Use either full name
        // or best to give offset from GMT.
        // Somehow the abbreviation UTC does not work.
        Calendar c = Calendar.getInstance(TimeZone.getTimeZone("GMT-07:00")); // PDT
        c.set(Calendar.YEAR, 2014);
        c.set(Calendar.MONTH, 6);   // month number - 1
        c.set(Calendar.DATE, 1);
        c.set(Calendar.HOUR_OF_DAY, 10);  // Use HOUR for 12 hour time.
        c.set(Calendar.MINUTE, 0);
        c.set(Calendar.SECOND, 0);
        Date start = c.getTime();
        int count = 0;
        try {
            Graph db = new Graph(graph_name, Graph.OpenOptions.ReadOnly);
            Transaction tx = new Transaction(db, false, true);
            StringID prop_id = new StringID(prop_name);
            for (NodeIterator ni = db.get_nodes(); !ni.done(); ni.next()) {
                Node n = ni.get_current();
                Property np;
                if ( (np = n.get_property(prop_id)) != null && np.type() == Property.Time) {
                    Date d = np.time_value().getDate();
                    if (d.after(start))
                        ++count;
                }
            }
            System.out.println("Count: " + count);
            if (count == expected)
                System.out.println("Test passed");
        } catch (jarvis.Exception e) {
            e.print();
            return;
        }
    }
}
