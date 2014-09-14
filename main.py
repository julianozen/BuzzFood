#!/usr/bin/env python
#
# Copyright 2007 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import webapp2
import os
import json
from google.appengine.ext import ndb
import urllib2
from google.appengine.api import urlfetch






class BaseHandler(webapp2.RequestHandler):
    # def render(self, template, **kw):
    #     self.response.out.write(render_str(template, **kw))
    def write(self, *a, **kw):
        self.response.out.write(*a, **kw)

class MainHandler(BaseHandler):
    def get(self):
        self.write('landing page!')

class SearchByCodeHandler(BaseHandler):
    def get(self):
        productid = self.request.get('q')
        response = FoodCatalog.query(FoodCatalog.productid == productid).fetch()
        results = []
        for food in response:
            json_result = {
                        'name':food.name,
                            'productid':food.productid,
                            'description':food.description,
                            'instruction_queue':food.instruction_queue,
                            'image':food.image,
                            'nutrition_info':food.nutrition_info,
                            'quantity':food.quantity,

                        }
            results.append(json_result)
        self.response.out.write(json.dumps(results))






class SearchByTextHandler(BaseHandler):
    def get(self):
        name = str(self.request.get('q')).lower()
        print name
        response = FoodCatalog.query(FoodCatalog.lower_name==name).fetch()
        results = []
        for food in response:
            json_result = {
                            'name':food.name,
                            'productid':food.productid,
                            'description':food.description,
                            'instruction_queue':food.instruction_queue,
                            'image':food.image,
                            'nutrition_info':food.nutrition_info,
                            'quantity':food.quantity,
                        }
            results.append(json_result)
        self.response.out.write(json.dumps(results))


#time cook, 3 , 0 , power, 9, start
#    t1234|p5|start


# http://localhost:11080/time?q=04963406&instruction_count=1
class TimerHandler(BaseHandler):
    def get(self):
        productid = self.request.get('q')
        instruction_count = int(self.request.get('instruction_count'))
        product = FoodCatalog.query(FoodCatalog.productid == productid).fetch()[0]
        instruction = product.instruction_queue[instruction_count]
        time = instruction['time']
        power = instruction['power']
        response = {'status':'succuess'}

        # Send commands to core
        command = 't'+time+"|p"+power
        self.response.out.write(json.dumps(response))


class StartHandler(BaseHandler):
    def get(self):
        response = {'status':'succuess'}
        # Send commands to core
        self.response.out.write(json.dumps(response))


class ClearHandler(BaseHandler):
    def get(self):
        response = {'status':'succuess'}

        # Send commands to core
        self.response.out.write(json.dumps(response))


class InstructionSender(webapp2.RequestHandler):
    def get(self):
        name = "Nuts"
        productid = "0090075140053"
        description = "Give me some nuts"
        instruction_queue = [{"type":"instruction",
                                "description":"Open Film, add water",
                                "image_url":"dsdksd.gif"},
                            {"type":"microwave", "time":45, "power":9},
                            {"type":"instruction",
                                "description":"Mix and Enjoy",
                                "image_url":"dsdksd.gif"},
                            {"type":"microwave", "time":30, "power":4},]
        image = "placeholder"
        nutrition_info = {"test":"null"}
        quantity = 2

        food= FoodCatalog(productid=productid, name=name, description=description, \
                instruction_queue=instruction_queue, image=image,
                nutrition_info=nutrition_info, quantity=quantity)
        food.put()
        self.response.write('added with power!')

    def post(self):
        time = self.request.get("time")
        for char in time:
            pass


class FoodCatalog(ndb.Model):
    productid = ndb.StringProperty(required=True)
    name = ndb.StringProperty(required=True)
    lower_name = ndb.ComputedProperty(
        lambda self: self.name.lower())
    description = ndb.TextProperty(required=True, default="")
    instruction_queue = ndb.JsonProperty(required=True, default=[])
    image = ndb.TextProperty(indexed=False)
    nutrition_info = ndb.JsonProperty(required=True, default=[])
    quantity = ndb.IntegerProperty(required=True)

app = webapp2.WSGIApplication([
    ('/?', MainHandler,),
    ('/code/?', SearchByCodeHandler,),
    ('/time/?', TimerHandler,),
    ('/search/?', SearchByTextHandler,),
    ('/start/?', StartHandler,),
    ('/add/?', InstructionSender,),
    ('/clear/?',ClearHandler)
], debug=True)
