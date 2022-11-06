/*
    These functions are used to generate updates to the page
    Thera are two types
    1) Blocks where the contents of a block are replaced by the results of the backend call
    2) Updates where the backend call returns json name value pairs. The names are matched
        to element ids, then the content of the individual elements replaced by the value
*/
/* ------------------- HELPERS --------------------*/
// find an element matching the provided id, and update it with the corresponding value
function bDsetIfExists(id, value) {
    var myEle = document.getElementById(id);
    if (myEle) {
        if (myEle.tagName === 'SELECT' || myEle.tagName === 'INPUT') {
            if (myEle.type == 'checkbox') {
                myEle.checked = (value == "true");
            }
            else {
                myEle.value = value;
            }
        }
        else {
            myEle.innerHTML = value;
        }
    }
    else {
        console.log(id + " not found");
    }
}

// work through a list of kvp updating each element with value
function bDupdatePropertiesFromList(obj) {
    for (var key of Object.keys(obj)) {
        bDsetIfExists(key, obj[key]);
    }
}


// check if an element exists with matching id, return string in the form &id=value
function bDvalueIfExists(id) {
    var docfield = document.getElementById(id);
    if (docfield) {
        return "&" + id + "=" + docfield.value;
    }
    return "";
}

//FUNCTIONALITY FOR FETCHING DATA

// call the url and replace content based on the returned name value list
async function bDcallAndReplaceFields(url) {
    var xhttp = new XMLHttpRequest();
    return new Promise(function (resolve, reject) {
        xhttp.onreadystatechange = function () {
            if (xhttp.readyState == 4) {
                if (xhttp.status == 200) {

                    var jsonResponse = JSON.parse(this.responseText);

                    for (var i = 0, len = jsonResponse.length; i < len; ++i) {
                        var update = jsonResponse[i];
                        bDupdatePropertiesFromList(update);
                    }
                    resolve("ok");
                }
                else {
                    bDsetIfExists("confirmation", "Error: " + xhttp.status + " " + xhttp.responseText);
                    reject(xhttp.responseText);
                }
            }
        };
        xhttp.open("GET", url);
        xhttp.send();
    })
}

async function bDgetDataForAction(action, refresh) {
    var sections = document.getElementsByClassName("UIsection");
    var fetched = 0;
    for (const ele of sections) {

        var UID = ele.getAttribute('UID');
        var sectionName = ele.getAttribute('name');

        if (UID && sectionName) {
            var url = "/page?action=" + action + "&UID=" + UID + "&div=" + sectionName;

            if (refresh) {
                url += "&bdRefresh=true"
            }
            var IDXAttr = ele.getAttribute('IDX');
            if (IDXAttr) {
                url += "&IDX=" + IDXAttr;
            }
            else {
                url += bDvalueIfExists("IDX");
            }

            try {
                fetched = fetched + await bDcallAndReplaceFields(url);
            }
            catch (err) {
                console.log(err)
            }
        }
    }
    return fetched;
}


function bDcustomDataAction(action) {
    bDgetDataForAction(action, false);
    return false;
}

// FUNCTIONALITY FOR LOADING BLOCKS

// find an element matching the provided id, and replace it with the corresponding value
async function bDcallAndReplaceBlock(id, url) {
    var xhttp = new XMLHttpRequest();
    return new Promise(function (resolve, reject) {
        xhttp.onreadystatechange = function () {
            if (xhttp.readyState == 4) {
                if (xhttp.status == 200) {

                    var newValue = xhttp.responseText;

                    if (newValue != "") {
                        bDsetIfExists(id, newValue);
                    }
                    resolve("ok");
                }
                else {
                    bDsetIfExists("confirmation", "Error: " + xhttp.status + " " + xhttp.responseText);
                    reject(xhttp.responseText);
                }
            }
        };
        xhttp.open("GET", url);
        xhttp.send();
    })
}

async function bDgetBlocks() {
    return new Promise(function (resolve, reject) {
        var blocks = document.getElementsByClassName("UIblock");
        var blockcount = 0;
        var fetched = 0;
        for (const ele of blocks) {

            blockcount++;

            var UID = ele.getAttribute('UID');
            var blockName = ele.getAttribute('name');
            var blockId = ele.getAttribute('id');
            if (UID && blockName && blockId) {
                var url = "/page?action=loadblock&UID=" + UID + "&block=" + blockId;

                var IDXAttr = ele.getAttribute('IDX');
                if (IDXAttr) {
                    url += "&IDX=" + IDXAttr;
                }
                else {
                    url += bDvalueIfExists("IDX");
                }

                try {
                    fetched = fetched +  bDcallAndReplaceBlock(blockId, url);
                }
                catch (err) {
                    console.log(err)
                }
            }
        }
        resolve(blockcount - fetched);
      
    })
}

// FUNCTIONALITY FOR SAVING DATA


function bDsavePageData(UIsectionId) {

    bDsetIfExists("confirmation", "");

    var uiSection = document.getElementById(UIsectionId);
    if (uiSection) {
        var UID = uiSection.getAttribute("UID");
        if (UID) {
            const input = uiSection.querySelector('form');
            if (input) {
                var inputData = new FormData(input);
                var request = new XMLHttpRequest();

                request.open("POST", "/page?action=save&UID=" + UID + "&div=" + UIsectionId);

                request.onreadystatechange = function () {
                    if (this.readyState == 4) {
                        if (this.status == 200) {
                            var jsonResponse = JSON.parse(this.responseText);

                            for (var i = 0, len = jsonResponse.length; i < len; ++i) {
                                var update = jsonResponse[i];
                                bDupdatePropertiesFromList(update);
                            }
                        }
                        else {
                            bDsetIfExists("confirmation", "Error: " + this.status + " " + this.responseText);
                        }
                    }
                };
                request.send(inputData);
            }
        }
    }
    return false;
}

// USER FACING FUNCTIONS

function bDloadPageData() {
    bDgetDataForAction("data", false);
}

function bDloadPageBlocks() {
    bDgetBlocks();
}

function bDrefreshData() {
    bDgetDataForAction("data", true);
};

async function bDloadBlocksThenData() {
    await bDgetBlocks().then(bDgetDataForAction("data", false));
}





// DOWNLOAD A FILE

function bDdownload(filename) {
    window.location = "/page?action=download&UID=307&file=" + filename;
}




function bDgrabFromURL(propName) {
    var TargetField = document.getElementById(propName);
    if (TargetField) {
        var url_string = window.location.href;
        var pageurl = new URL(url_string);
        var urlField = pageurl.searchParams.get(propName);
        if (urlField) {
            TargetField.value = urlField;
        }
    }
}

