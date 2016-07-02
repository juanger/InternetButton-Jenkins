import groovy.json.JsonBuilder

// Change to your device ID
def deviceId = "DEVICE_ID";
// Change to your particle.io access token
def accessToken = "TOKEN";

def viewName = manager.envVars["VIEW_NAME"]
def currentName = manager.envVars["JOB_NAME"]
def unstableFlag = false
def failedFlag = false
def failedHighFlag = false
def failedCount = 0
def unstableCount = 0

// Loop through all the jobs in the view (excluding this job) and count failed/unstable jobs.
hudson.model.Hudson.instance.getView(viewName).items.each() {
    if (it.isBuildable() && it.fullDisplayName != currentName && it.lastCompletedBuild) {
        def lastResult = it.lastCompletedBuild.result.toString()
        def failed = lastResult.equals("FAILURE")
        def unstable = lastResult.equals("UNSTABLE")
        unstableFlag = unstableFlag ||  unstable
        failedFlag = failedFlag || failed

        if (failed) {
            failedCount = failedCount + 1
        }

        if (unstable) {
            unstableCount = unstableCount + 1
        }

        if (failed && (it.fullDisplayName.endsWith("_package") || it.fullDisplayName.endsWith("test_users"))) {
            failedHighFlag = true
        }
    }
}

if (failedFlag || unstableFlag) {
    manager.buildFailure()
}

def command = "curl https://api.particle.io/v1/devices/"+deviceId+"/failed -d arg="+failedCount+","+unstableCount+" -d access_token="+accessToken
println command.execute().text
