Udm validator stability Test:

Using injector tool [injector tool](https://gerrit.ericsson.se/plugins/gitiles/HSS/CCSM/cpph2/+/master/test/injector/), a performance test has been made on the eric-udm-udmprovisioningvalidator service.
Command used: ./injector.usvc.elf -u 'http://eric-udm-udmprovisioningvalidator:9002/validation/v1/validate/validate' -m POST -f 2000 -d 3600 -j UdmValidatorExample.json -M 10
Using the following parameters:
    • Duration: 1 hour
    • Data: {
  "changes": [
    {
      "operation": "CREATE",
      "resource_path": "/subscribers/2208a/udmSubscription/udmStaticAndRegistrationData/udmStaticData",
      "data": {
        "udmSliceProfileId": {
          "id": "1"
        },
        "subscribedUeAmbr": {
          "downlink": "1 Gbps",
          "uplink": "2 Mbps"
        },
        "ratRestrictions": [
          "NR",
          "WLAN"
        ],
        "forbiddenAreas": [
          {
            "id": "1"
          }
        ],
        "serviceAreaRestriction": {
          "restrictionType": "ALLOWED_AREAS",
          "areas": [
            {
              "id": "1"
            }
          ],
          "maxNumOfTAs": 100
        },
        "coreNetworkTypeRestrictions": [
          "5GC",
          "EPC"
        ],
        "mpsPriority": true,
        "mcsPriority": false,
        "dlPacketCount": 3,
        "micoAllowed": false,
        "staticIpAddress": {
          "nssaiDnnIpLists": [
            {
              "nssai": {
                "sst": 14,
                "sd": "ABCDEF"
              },
              "dnnIpList": [
                {
                  "dnn": "5",
                  "ipAddress": [
                    {
                      "ipv4Addr": "192.168.10.1"
                    },
                    {
                      "ipv6Addr": "2001:db8:85a3::8a2e:370:7334"
                    }
                  ]
                },
                {
                  "dnn": "7",
                  "ipAddress": [
                    {
                      "ipv4Addr": "192.168.10.2"
                    },
                    {
                      "ipv6Addr": "2001:db8:85a3::8a2e:370:7335"
                    }
                  ]
                }
              ]
            },
            {
              "nssai": {
                "sst": 15,
                "sd": "ABCEEE"
              },
              "dnnIpList": [
                {
                  "dnn": "6",
                  "ipAddress": [
                    {
                      "ipv4Addr": "192.168.10.3"
                    },
                    {
                      "ipv6Addr": "2001:db8:85a3::8a2e:370:7336"
                    }
                  ]
                },
                {
                  "dnn": "8",
                  "ipAddress": [
                    {
                      "ipv4Addr": "192.168.10.4"
                    },
                    {
                      "ipv6Addr": "2001:db8:85a3::8a2e:370:7337"
                    }
                  ]
                }
              ]
            }
          ]
        },
        "roamingOdb": "OUTSIDE_HOME_PLMN",
        "ueUsageType": 23,
        "rfspIndex": 98,
        "subsRegTimer": 102,
        "udmSliceData": {
          "plmnIdSets": [
            {
              "plmnIdSetIdentifier": "12345",
              "nssai": {
                "defaultSingleNssais": [
                  {
                    "dnnListId": "Default",
                    "nssai": {
                      "sst": 5,
                      "sd": "AAAAAA"
                    }
                  },
                  {
                    "dnnListId": "1",
                    "nssai": {
                      "sst": 3,
                      "sd": "EEEAAA"
                    }
                  }
                ],
                "singleNssais": [
                  {
                    "dnnListId": "Default",
                    "nssai": {
                      "sst": 5
                    }
                  },
                  {
                    "dnnListId": "4",
                    "nssai": {
                      "sst": 4,
                      "sd": "ABCCBA"
                    }
                  }
                ]
              },
              "dnnLists": [
                {
                  "dnnListId": "1",
                  "dnnDataList": [
                    {
                      "dnn": "Default",
                      "defaultDnnIndicator": true,
                      "lboRoamingAllowed": true,
                      "iwkEpsInd": false,
                      "ladnIndicator": false,
                      "sscModes": {
                        "defaultSscMode": "SSC_MODE_1",
                        "allowedSscModes": [
                          "SSC_MODE_2"
                        ]
                      },
                      "pduSessionTypes": {
                        "defaultSessionType": "IPV6",
                        "allowedSessionTypes": [
                          "IPV4"
                        ]
                      },
                      "5gQosProfile": {
                        "5qi": 235,
                        "priorityLevel": 112,
                        "arp": {
                          "priorityLevel": 13,
                          "preemptCap": "NOT_PREEMPT",
                          "preemptVuln": "NOT_PREEMPTABLE"
                        }
                      }
                    }
                  ]
                },
                {
                  "dnnListId": "4",
                  "dnnDataList": [
                    {
                      "dnn": "DNN4",
                      "defaultDnnIndicator": true,
                      "lboRoamingAllowed": false,
                      "iwkEpsInd": true,
                      "ladnIndicator": true,
                      "sscModes": {
                        "defaultSscMode": "SSC_MODE_2",
                        "allowedSscModes": [
                          "SSC_MODE_1"
                        ]
                      },
                      "pduSessionTypes": {
                        "defaultSessionType": "IPV4",
                        "allowedSessionTypes": [
                          "IPV6"
                        ]
                      },
                      "5gQosProfile": {
                        "5qi": 230,
                        "priorityLevel": 110,
                        "arp": {
                          "priorityLevel": 12,
                          "preemptCap": "NOT_PREEMPT",
                          "preemptVuln": "NOT_PREEMPTABLE"
                        }
                      }
                    }
                  ]
                }
              ]
            },
            {
              "plmnIdSetIdentifier": "12346",
              "nssai": {
                "defaultSingleNssais": [
                  {
                    "dnnListId": "Default",
                    "nssai": {
                      "sst": 6,
                      "sd": "BBBBBB"
                    }
                  },
                  {
                    "dnnListId": "10",
                    "nssai": {
                      "sst": 4,
                      "sd": "FFFBBB"
                    }
                  }
                ],
                "singleNssais": [
                  {
                    "dnnListId": "Default",
                    "nssai": {
                      "sst": 6
                    }
                  },
                  {
                    "dnnListId": "40",
                    "nssai": {
                      "sst": 8,
                      "sd": "BBBCCC"
                    }
                  }
                ]
              },
              "dnnLists": [
                {
                  "dnnListId": "10",
                  "dnnDataList": [
                    {
                      "dnn": "Default",
                      "defaultDnnIndicator": true,
                      "lboRoamingAllowed": true,
                      "iwkEpsInd": false,
                      "ladnIndicator": false,
                      "sscModes": {
                        "defaultSscMode": "SSC_MODE_1",
                        "allowedSscModes": [
                          "SSC_MODE_2"
                        ]
                      },
                      "pduSessionTypes": {
                        "defaultSessionType": "IPV6",
                        "allowedSessionTypes": [
                          "IPV4"
                        ]
                      },
                      "5gQosProfile": {
                        "5qi": 236,
                        "priorityLevel": 113,
                        "arp": {
                          "priorityLevel": 14,
                          "preemptCap": "NOT_PREEMPT",
                          "preemptVuln": "NOT_PREEMPTABLE"
                        }
                      }
                    }
                  ]
                },
                {
                  "dnnListId": "40",
                  "dnnDataList": [
                    {
                      "dnn": "DNN1",
                      "defaultDnnIndicator": true,
                      "lboRoamingAllowed": true,
                      "iwkEpsInd": true,
                      "ladnIndicator": true,
                      "sscModes": {
                        "defaultSscMode": "SSC_MODE_2",
                        "allowedSscModes": [
                          "SSC_MODE_1"
                        ]
                      },
                      "pduSessionTypes": {
                        "defaultSessionType": "IPV4",
                        "allowedSessionTypes": [
                          "IPV6"
                        ]
                      },
                      "5gQosProfile": {
                        "5qi": 231,
                        "priorityLevel": 111,
                        "arp": {
                          "priorityLevel": 13,
                          "preemptCap": "NOT_PREEMPT",
                          "preemptVuln": "NOT_PREEMPTABLE"
                        }
                      }
                    }
                  ]
                }
              ]
            }
          ]
        }
      }
    }
  ],
  "relatedResources": {
    "/subscribers/2208a/journal/provJournal": {
      "imsi": "IMSI1",
      "notifRef": "notifRef1",
      "imsiMask": "imsiMask1"
    },
    "/profiles/udmProfiles/udmSliceProfiles/1111": {
      "udmSliceProfileId": "1",
      "plmnIdSets": [
        {
          "plmnIdSetIdentifier": "Default",
          "nssai": {
            "defaultSingleNssais": [
              {
                "dnnListId": "Default",
                "nssai": {
                  "sst": 3,
                  "sd": "ABCDEF"
                }
              }
            ],
            "singleNssais": [
              {
                "dnnListId": "Default",
                "nssai": {
                  "sst": 4
                }
              }
            ]
          },
          "dnnLists": [
            {
              "dnnListId": "Default",
              "dnnDataList": [
                {
                  "dnn": "Default",
                  "defaultDnnIndicator": true,
                  "lboRoamingAllowed": false,
                  "iwkEpsInd": false,
                  "ladnIndicator": false,
                  "sscModes": {
                    "defaultSscMode": "SSC_MODE_1",
                    "allowedSscModes": [
                      "SSC_MODE_2"
                    ]
                  },
                  "pduSessionTypes": {
                    "defaultSessionType": "IPV4",
                    "allowedSessionTypes": [
                      "IPV6"
                    ]
                  },
                  "5gQosProfile": {
                    "5qi": 233,
                    "priorityLevel": 110,
                    "arp": {
                      "priorityLevel": 11,
                      "preemptCap": "NOT_PREEMPT",
                      "preemptVuln": "NOT_PREEMPTABLE"
                    }
                  }
                }
              ]
            },
            {
              "dnnListId": "1",
              "dnnDataList": [
                {
                  "dnn": "Default",
                  "defaultDnnIndicator": true,
                  "lboRoamingAllowed": false,
                  "iwkEpsInd": true,
                  "ladnIndicator": false,
                  "sscModes": {
                    "defaultSscMode": "SSC_MODE_1",
                    "allowedSscModes": [
                      "SSC_MODE_2"
                    ]
                  },
                  "pduSessionTypes": {
                    "defaultSessionType": "IPV6",
                    "allowedSessionTypes": [
                      "IPV4"
                    ]
                  },
                  "5gQosProfile": {
                    "5qi": 220,
                    "priorityLevel": 105,
                    "arp": {
                      "priorityLevel": 9,
                      "preemptCap": "NOT_PREEMPT",
                      "preemptVuln": "NOT_PREEMPTABLE"
                    }
                  }
                }
              ]
            }
          ]
        },
        {
          "plmnIdSetIdentifier": "12345",
          "nssai": {
            "defaultSingleNssais": [
              {
                "dnnListId": "Default",
                "nssai": {
                  "sst": 3,
                  "sd": "ABCDEF"
                }
              }
            ],
            "singleNssais": [
              {
                "dnnListId": "Default",
                "nssai": {
                  "sst": 4
                }
              }
            ]
          },
          "dnnLists": [
            {
              "dnnListId": "Default",
              "dnnDataList": [
                {
                  "dnn": "Default",
                  "defaultDnnIndicator": true,
                  "lboRoamingAllowed": false,
                  "iwkEpsInd": false,
                  "ladnIndicator": false,
                  "sscModes": {
                    "defaultSscMode": "SSC_MODE_1",
                    "allowedSscModes": [
                      "SSC_MODE_2"
                    ]
                  },
                  "pduSessionTypes": {
                    "defaultSessionType": "IPV4",
                    "allowedSessionTypes": [
                      "IPV6"
                    ]
                  },
                  "5gQosProfile": {
                    "5qi": 233,
                    "priorityLevel": 110,
                    "arp": {
                      "priorityLevel": 11,
                      "preemptCap": "NOT_PREEMPT",
                      "preemptVuln": "NOT_PREEMPTABLE"
                    }
                  }
                }
              ]
            },
            {
              "dnnListId": "1",
              "dnnDataList": [
                {
                  "dnn": "Default",
                  "defaultDnnIndicator": true,
                  "lboRoamingAllowed": false,
                  "iwkEpsInd": true,
                  "ladnIndicator": false,
                  "sscModes": {
                    "defaultSscMode": "SSC_MODE_1",
                    "allowedSscModes": [
                      "SSC_MODE_2"
                    ]
                  },
                  "pduSessionTypes": {
                    "defaultSessionType": "IPV6",
                    "allowedSessionTypes": [
                      "IPV4"
                    ]
                  },
                  "5gQosProfile": {
                    "5qi": 220,
                    "priorityLevel": 105,
                    "arp": {
                      "priorityLevel": 9,
                      "preemptCap": "NOT_PREEMPT",
                      "preemptVuln": "NOT_PREEMPTABLE"
                    }
                  }
                }
              ]
            }
          ]
        },
        {
          "plmnIdSetIdentifier": "12346",
          "nssai": {
            "defaultSingleNssais": [
              {
                "dnnListId": "Default",
                "nssai": {
                  "sst": 3,
                  "sd": "ABCDEF"
                }
              }
            ],
            "singleNssais": [
              {
                "dnnListId": "Default",
                "nssai": {
                  "sst": 4
                }
              }
            ]
          },
          "dnnLists": [
            {
              "dnnListId": "Default",
              "dnnDataList": [
                {
                  "dnn": "Default",
                  "defaultDnnIndicator": true,
                  "lboRoamingAllowed": false,
                  "iwkEpsInd": false,
                  "ladnIndicator": false,
                  "sscModes": {
                    "defaultSscMode": "SSC_MODE_1",
                    "allowedSscModes": [
                      "SSC_MODE_2"
                    ]
                  },
                  "pduSessionTypes": {
                    "defaultSessionType": "IPV4",
                    "allowedSessionTypes": [
                      "IPV6"
                    ]
                  },
                  "5gQosProfile": {
                    "5qi": 233,
                    "priorityLevel": 110,
                    "arp": {
                      "priorityLevel": 11,
                      "preemptCap": "NOT_PREEMPT",
                      "preemptVuln": "NOT_PREEMPTABLE"
                    }
                  }
                }
              ]
            }
          ]
        }
      ]
    }
  }
}
 



    • fixed Rate: 2000 reqs/s
    • number of parallel streams: 10

Results:

Results for the uservice with log level “info”:
------------------------------ params ------------------------------
URI                 :http://127.0.0.1:9002/validation/v1/validate/validate
Method              :POST
JSON file           :udmprovdata.json
duration(sg)        :3600
rate(reqs./sg)      :2000
sleep time(us)      :500
max parallel streams:10
stuck timeout(sg)   :1
------------------------------ params ------------------------------
current rate: 002000.92 reqs/sg.
  sending complete!!!
reception complete!!!
------------------------------ stats -------------------------------
  elapsed req ms:  3600917
  elapsed rsp ms:  3600917
mean time req ns:   499.77
mean time rsp ns:   499.77
       (req)./sg:  2000.92
 (req<=>rsp)./sg:  2000.92

total rsp:7205139 err:226(0.00%) 2xx:7204913(100.00%) 4xx:0(0.00%) 500:0(0.00%) 501:0(0.00%) 502:0(0.00%) 503:0(0.00%) 504:0(0.00%)

mean per response type latency (ms) / standard deviation (ms) :
 2xx:     1.11856 / sd:     2.69295
 4xx:           - / sd:           -
 500:           - / sd:           -
 501:           - / sd:           -
 502:           - / sd:           -
 503:           - / sd:           -
 504:           - / sd:           -

mean per response type database latency (ms) / standard deviation (ms) :
 2xx:     0.00000 / sd:     0.00000
 4xx:           - / sd:           -
 500:           - / sd:           -

------------------------------ stats -------------------------------


Results for the uservice with log level “debug”:

------------------------------ params ------------------------------
URI                 :http://127.0.0.1:9002/validation/v1/validate/validate
Method              :POST
JSON file           :udmprovdata.json
duration(sg)        :3600
rate(reqs./sg)      :2000
sleep time(us)      :500
max parallel streams:10
stuck timeout(sg)   :1
------------------------------ params ------------------------------
current rate: 001316.79 reqs/sg.
  sending complete!!!
reception complete!!!
------------------------------ stats -------------------------------
  elapsed req ms:  3600900
  elapsed rsp ms:  3600903
mean time req ns:   759.42
mean time rsp ns:   759.42
       (req)./sg:  1316.79
 (req<=>rsp)./sg:  1316.79

total rsp:4741637 err:0(0.00%) 2xx:4739308(99.95%) 4xx:0(0.00%) 500:0(0.00%) 501:0(0.00%) 502:0(0.00%) 503:2330(0.05%) 504:0(0.00%)

mean per response type latency (ms) / standard deviation (ms) :
 2xx:     6.76153 / sd:    12.75734
 4xx:           - / sd:           -
 500:           - / sd:           -
 501:           - / sd:           -
 502:           - / sd:           -
 503:     0.86441 / sd:     6.74468
 504:           - / sd:           -

mean per response type database latency (ms) / standard deviation (ms) :
 2xx:     0.00000 / sd:     0.00000
 4xx:           - / sd:           -
 500:           - / sd:           -

Problem Details:
----------------
503:
 Requests:[000002330]; Title:[The NF experiences congestion and performs overload control, which does not allow the request to be processed.]

------------------------------ stats -------------------------------

The specifications of the machine where the test was executed are:

8 cpus Intel(R) Xeon(R) Gold 6132 CPU @ 2.60GHz
32 GB of RAM
